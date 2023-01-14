#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 1024;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << std::endl;
        return 0;
    }

    // 创建UDP套接字
    int servSock = socket(PF_INET, SOCK_DGRAM, 0);
    if (servSock == -1) {
        std::cout << "socket 错误" << std::endl;
        return 0;
    }

    // 初始化UDP服务端地址信息
    sockaddr_in servAdr;
    std::memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(std::atoi(argv[1]));

    // 绑定UDP地址信息
    int stu = bind(servSock, (sockaddr*)&servAdr, sizeof(servAdr));
    if (stu == -1) {
        close(servSock);
        std::cout << "bind 错误" << std::endl;
        return 0;
    }

    char msg[BUF_SIZE] = { 0 };

    while (true) {
        sockaddr_in clntAdr;
        socklen_t szClntAdr = sizeof(clntAdr);
        int strLen = recvfrom(servSock, msg, BUF_SIZE, 0, (sockaddr*)&clntAdr, &szClntAdr);
        if (strLen == -1) {
            std::cout << "recvfrom 错误" << std::endl;
            continue;
        }
        std::string adrStr = inet_ntoa(clntAdr.sin_addr);
        msg[strLen] = 0;
        std::cout << "收到客户端" << adrStr << "的消息：" << msg << std::endl;
        sendto(servSock, msg, strLen, 0, (sockaddr*)&clntAdr, szClntAdr);
    }

    close(servSock);

    return 0;
}

