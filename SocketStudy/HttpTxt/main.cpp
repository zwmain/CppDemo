#include <arpa/inet.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <linux/in.h>
#include <string>
#include <sys/endian.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 32;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "参数错误，正确用法为：" << argv[0] << " <PORT>" << std::endl;
        return 0;
    }

    int servSock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in adr;
    std::memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    int port = std::atoi(argv[1]);
    adr.sin_port = htons(port);

    int rtn = bind(servSock, (sockaddr*)&adr, sizeof(adr));
    if (rtn == -1) {
        std::cout << "bind 错误：" << port << std::endl;
        close(servSock);
        return 0;
    }

    rtn = listen(servSock, 5);
    if (rtn == -1) {
        std::cout << "listen 错误：" << port << std::endl;
        close(servSock);
        return 0;
    }

    sockaddr_in clntAdr;
    std::memset(&clntAdr, 0, sizeof(clntAdr));
    socklen_t clntLen = sizeof(clntAdr);
    int clntSock = accept(servSock, (sockaddr*)&clntAdr, &clntLen);
    std::cout << "接收到连接" << clntSock << std::endl;

    char buf[BUF_SIZE] = { 0 };
    while (true) {
        int n = read(clntSock, buf, BUF_SIZE - 1);
        buf[n] = 0;
        std::cout << buf;
    }

    std::cout << "退出" << std::endl;
    close(clntSock);
    close(servSock);

    return 0;
}

