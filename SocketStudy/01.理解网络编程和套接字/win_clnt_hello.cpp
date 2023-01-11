#include <WinSock2.h>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " IP port" << std::endl;
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
    SOCKET clntSock = socket(PF_INET, SOCK_STREAM, 0);
    if (clntSock == INVALID_SOCKET) {
        std::cout << "socket 错误" << std::endl;
        return 0;
    }

    // 初始化服务端地址信息
    sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(std::atoi(argv[2]));

    // 连接到服务端
    stu = connect(clntSock, (sockaddr*)&servAddr, sizeof(servAddr));
    if (stu == SOCKET_ERROR) {
        closesocket(clntSock);
        std::cout << "connect 错误" << std::endl;
        return 0;
    }

    char msg[30] = { 0 };
    int strLen = recv(clntSock, msg, sizeof(msg) - 1, 0);
    if (strLen == -1) {
        closesocket(clntSock);
        std::cout << "recv 错误" << std::endl;
        return 0;
    }

    std::cout << "接收：" << msg << std::endl;
    closesocket(clntSock);

    WSACleanup();

    return 0;
}
