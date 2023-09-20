#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <linux/in.h>
#include <string>
#include <sys/endian.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 32;

// 封装函数，设置套接字非阻塞
void setNoBlockingMode(int fd)
{
    int flag = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

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
    setNoBlockingMode(clntSock);
    std::cout << "接收到连接" << clntSock << std::endl;

    char buf[BUF_SIZE] = { 0 };
    while (true) {
        int n = read(clntSock, buf, BUF_SIZE - 1);
        if (n == 0) {
            std::cout << "客户端：" << clntSock << "断开连接" << std::endl;
            break;
        } else if (n < 0) {
            if (errno == EAGAIN) {
                std::cout << "读取完毕" << std::endl;
            } else {
                std::cout << "读取错误" << std::endl;
            }
            break;
        } else {
            buf[n] = 0;
            std::cout << buf;
        }
    }

    const std::string resHeader = "HTTP/1.1 200 OK\r\n"
                                  "Content-Type: text/html;charset=utf-8\r\n"
                                  "\r\n";
    const std::string resBody = "<!DOCTYPE html>\r\n"
                                "<html>\r\n"
                                "<head><title>HttpTxt</title></head>\r\n"
                                "<body>简单的HTTP响应</body>\r\n"
                                "</html>\r\n";

    write(clntSock, resHeader.c_str(), resHeader.size());
    write(clntSock, resBody.c_str(), resBody.size());

    std::cout << "退出" << std::endl;
    close(clntSock);
    close(servSock);

    return 0;
}

