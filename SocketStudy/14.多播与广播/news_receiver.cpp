#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

constexpr int BUF_SIZE = 30;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <GroupIP> <PORT>" << std::endl;
        return 0;
    }

    int recv_sock = socket(PF_INET, SOCK_DGRAM, 0);

    sockaddr_in adr;
    std::memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(std::atoi(argv[2]));

    int rtn = bind(recv_sock, (sockaddr*)&adr, sizeof(adr));
    if (rtn == -1) {
        std::cout << "bind error";
        return 0;
    }

    ip_mreq join_adr;
    std::memset(&join_adr, 0, sizeof(join_adr));
    join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &join_adr, sizeof(join_adr));

    char buf[BUF_SIZE] = { 0 };
    while (true) {
        int str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, nullptr, 0);
        if (str_len < 0) {
            break;
        }
        buf[str_len] = 0;
        std::cout << buf << std::endl;
    }

    close(recv_sock);

    return 0;
}
