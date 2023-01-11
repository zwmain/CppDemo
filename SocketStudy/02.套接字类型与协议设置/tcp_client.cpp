#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " IP port" << std::endl;
        return 0;
    }

    // 调用socket创建套接字
    int clntSock = socket(PF_INET, SOCK_STREAM, 0);
    if (clntSock == -1) {
        std::cout << "套接字创建失败！" << std::endl;
        return 0;
    }

    // 生成服务器地址信息
    sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(std::atoi(argv[2]));

    // 连接到服务器
    int stu = connect(clntSock, (sockaddr*)&servAddr, sizeof(servAddr));
    if (stu == -1) {
        close(clntSock);
        std::cout << "connect 错误！" << std::endl;
        return 0;
    }

    char msg[30] = { 0 };
    int strLen = read(clntSock, msg, sizeof(msg) - 1);
    if (strLen == -1) {
        std::cout << "read 错误！" << std::endl;
        return 0;
    }

    std::cout << msg << std::endl;

    close(clntSock);

    return 0;
}

