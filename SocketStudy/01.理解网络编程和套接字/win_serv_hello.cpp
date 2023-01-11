#include <WinSock2.h>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " port" << std::endl;
        return 0;
    }

    // 初始化库
    WSADATA wsaData;
    int stu = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (stu != 0) {
        std::cout << "WSAStartup 错误：" << stu << std::endl;
        return 0;
    }

    // 创建socket
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, 0);
    if (servSock == INVALID_SOCKET) {
        std::cout << "socket 错误" << std::endl;
        return 0;
    }

    // 初始化服务器地址信息
    sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(std::atoi(argv[1]));

    // 调用bind函数绑定地址信息
    stu = bind(servSock, (sockaddr*)&servAddr, sizeof(servAddr));
    if (stu == SOCKET_ERROR) {
        closesocket(servSock);
        std::cout << "bind 错误" << std::endl;
        return 0;
    }

    // 调用listen函数，进入监听状态
    stu = listen(servSock, 5);
    if (stu == SOCKET_ERROR) {
        closesocket(servSock);
        std::cout << "listen 错误" << std::endl;
        return 0;
    }

    sockaddr_in clntAddr;
    int clntAddrSize = sizeof(clntAddr);
    // 接收请求
    SOCKET clntSock = accept(servSock, (sockaddr*)&clntAddr, &clntAddrSize);
    if (clntSock == INVALID_SOCKET) {
        closesocket(servSock);
        std::cout << "accept 错误" << std::endl;
        return 0;
    }

    std::string msg = "Hello World!";
    send(clntSock, msg.c_str(), msg.size(), 0);

    closesocket(clntSock);
    closesocket(servSock);

    // 清理库
    WSACleanup();

    return 0;
}
