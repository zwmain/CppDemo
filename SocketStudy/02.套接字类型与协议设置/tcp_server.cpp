#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " port" << std::endl;
        return 0;
    }

    // 调用socket函数创建套接字
    int servSock = socket(PF_INET, SOCK_STREAM, 0);
    if (servSock == -1) {
        std::cout << "套接字创建失败！" << std::endl;
        return 0;
    }

    // 初始化地址信息
    sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(std::atoi(argv[1]));

    // 调用bind函数绑定IP地址和端口号
    int stu = bind(servSock, (sockaddr*)&servAddr, sizeof(servAddr));
    if (stu == -1) {
        close(servSock);
        std::cout << "bind 错误" << std::endl;
        return 0;
    }

    // 调用listen函数进入可接收请求的监听状态
    stu = listen(servSock, 5);
    if (stu == -1) {
        close(servSock);
        std::cout << "listen 错误" << std::endl;
        return 0;
    }

    // 调用accept函数受理连接请求
    sockaddr_in clntAddr;
    socklen_t clntAddrSize = sizeof(clntAddr);
    int clntSock = accept(servSock, (sockaddr*)&clntAddr, &clntAddrSize);
    if (clntSock == -1) {
        close(servSock);
        std::cout << "accept 错误" << std::endl;
        return 0;
    }

    std::string msg="Hello World!";
    write(clntSock,msg.c_str(),msg.size());

    close(clntSock);
    close(servSock);

    return 0;
}

