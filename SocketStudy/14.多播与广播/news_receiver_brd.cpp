#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <linux/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

constexpr int BUF_SIZE = 30;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <PORT>" << std::endl;
        return 0;
    }

    int recv_sock = socket(PF_INET, SOCK_DGRAM, 0);

    sockaddr_in adr;
    std::memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    int port = std::atoi(argv[1]);
    adr.sin_port = htons(port);
    std::cout << "PORT: " << port << std::endl;

    int rtn = bind(recv_sock, (sockaddr*)&adr, sizeof(adr));
    if (rtn == -1) {
        std::cout << "bind error" << std::endl;
        return 0;
    }

    char buf[BUF_SIZE] = { 0 };
    while (true) {
        sockaddr_in src_adr;
        socklen_t sl = 0;
        int l = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, (sockaddr*)&src_adr, &sl);
        std::string srcIp = inet_ntoa(src_adr.sin_addr);
        std::cout << srcIp << " - ";
        if (l < 0) {
            break;
        }
        buf[l] = 0;
        std::cout << buf;
    }

    close(recv_sock);

    return 0;
}

