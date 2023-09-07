#include <WS2tcpip.h>
#include <WinSock2.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

constexpr int BUF_SIZE = 30;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <PORT>" << std::endl;
        return 0;
    }

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSA error" << std::endl;
        return 0;
    }

    SOCKET recvSock = socket(PF_INET, SOCK_DGRAM, 0);
    SOCKADDR_IN adr;
    std::memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(std::atoi(argv[1]));

    int rtn = bind(recvSock, (SOCKADDR*)&adr, sizeof(adr));
    if (rtn == SOCKET_ERROR) {
        std::cout << "bind error" << std::endl;
        return 0;
    }

    char buf[BUF_SIZE] = { 0 };
    while (true) {
        int strLen = recvfrom(recvSock, buf, BUF_SIZE - 1, 0, nullptr, 0);
        if (strLen < 0) {
            break;
        }
        buf[strLen] = 0;
        std::cout << buf;
    }

    closesocket(recvSock);
    WSACleanup();

    return 0;
}
