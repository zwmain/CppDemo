#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 1024;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " port" << std::endl;
        return 0;
    }

    // 创建套接字
    int servSock = socket(PF_INET, SOCK_STREAM, 0);
    if (servSock == -1) {
        std::cout << "socket 错误！" << std::endl;
        return 0;
    }

    // 初始化地址信息
    sockaddr_in servAdr;
    std::memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(std::atoi(argv[1]));

    // 绑定套接字地址信息
    int stu = bind(servSock, (sockaddr*)&servAdr, sizeof(servAdr));
    if (stu == -1) {
        close(servSock);
        std::cout << "bind 错误！" << std::endl;
        return 0;
    }

    // 进入监听状态
    stu = listen(servSock, 5);
    if (stu == -1) {
        close(servSock);
        std::cout << "listen 错误！" << std::endl;
        return 0;
    }

    char buf[BUF_SIZE] = { 0 };

    // 循环接收请求，只接收5次
    for (int i = 0; i < 5; ++i) {
        sockaddr_in clntAdr;
        socklen_t szClntAdr = sizeof(clntAdr);
        int clntSock = accept(servSock, (sockaddr*)&clntAdr, &szClntAdr);

        if (stu == -1) {
            std::cout << "第" << i << "个客户端"
                      << "accept 错误！" << std::endl;
            continue;
        } else {
            std::string adrStr = inet_ntoa(clntAdr.sin_addr);
            std::cout << "接收第" << i << "个客户端"
                      << ":" << adrStr << std::endl;
        }

        int strLen = 0;
        do {
            strLen = read(clntSock, buf, BUF_SIZE);
            if (strLen == -1) {
                std::cout << "第" << i << "个客户端 read 错误" << std::endl;
                break;
            }
            write(clntSock, buf, strLen);
        } while (strLen != 0);

        close(clntSock);
        std::cout << "第" << i << "个客户端通信完毕" << std::endl;
    }

    close(servSock);

    return 0;
}

