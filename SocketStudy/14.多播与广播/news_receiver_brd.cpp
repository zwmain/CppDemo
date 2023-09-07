#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <linux/in.h>
#include <sys/socket.h>
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
    adr.sin_port = htons(std::atoi(argv[1]));

    int rtn = bind(recv_sock, (sockaddr*)&adr, sizeof(adr));
    if (rtn == -1) {
        std::cout << "bind error" << std::endl;
        return 0;
    }

    char buf[BUF_SIZE] = { 0 };
    while (true) {
        int l = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, nullptr, 0);
        if (l < 0) {
            break;
        }
        buf[l] = 0;
        std::cout << buf;
    }

    close(recv_sock);

    return 0;
}

