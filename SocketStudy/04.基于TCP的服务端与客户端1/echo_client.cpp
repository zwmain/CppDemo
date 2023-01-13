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

    // 创建套接字
    int clntSock = socket(PF_INET, SOCK_STREAM, 0);
    if (clntSock == -1) {
        std::cout << "socket 错误" << std::endl;
        return 0;
    }

    // 初始化服务器地址信息
    sockaddr_in servAdr;
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);
    servAdr.sin_port = htons(std::atoi(argv[2]));

    // 连接到服务器
    int stu = connect(clntSock, (sockaddr*)&servAdr, sizeof(servAdr));
    if (stu == -1) {
        close(clntSock);
        std::cout << "connect 错误" << std::endl;
        return 0;
    }

    char buf[BUF_SIZE] = { 0 };

    while (true) {
        std::cout << "输入消息：";
        std::string msg;
        std::cin >> msg;
        if (msg == "Q" || msg == "q") {
            break;
        }

        write(clntSock, msg.c_str(), msg.size());
        int strLen = read(clntSock, buf, BUF_SIZE - 1);
        buf[strLen] = 0;
        std::cout << "服务端消息：" << buf << std::endl;
    }

    close(clntSock);
    return 0;
}

