#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 1024;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " IP port" << std::endl;
        return 0;
    }

    // 创建UDP套接字
    int clntSock = socket(PF_INET, SOCK_DGRAM, 0);
    if (clntSock == -1) {
        std::cout << "socket 错误" << std::endl;
        return 0;
    }

    // 初始化目标服务端地址信息
    sockaddr_in servAdr;
    std::memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);
    servAdr.sin_port = htons(std::atoi(argv[2]));

    char buf[BUF_SIZE] = { 0 };
    // 不用连接，直接发送消息
    while (true) {
        std::cout << "输入信息：";
        std::string msg;
        std::cin >> msg;
        if (msg == "Q" || msg == "q") {
            break;
        }

        sendto(clntSock, msg.c_str(), msg.size(), 0, (sockaddr*)&servAdr, sizeof(servAdr));
        socklen_t szAdr = sizeof(servAdr);
        int strLen = recvfrom(clntSock, buf, BUF_SIZE, 0, (sockaddr*)&servAdr, &szAdr);
        buf[strLen] = 0;
        std::string adrStr = inet_ntoa(servAdr.sin_addr);
        std::cout << "从" << adrStr << "收到消息：" << buf << std::endl;
    }

    close(clntSock);

    return 0;
}

