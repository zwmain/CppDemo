#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 30;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <PORT>" << std::endl;
        return 0;
    }

    int acpt_sock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in acpt_adr;
    std::memset(&acpt_adr, 0, sizeof(acpt_adr));
    acpt_adr.sin_family = AF_INET;
    acpt_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_adr.sin_port = htons(std::atoi(argv[1]));

    int rtn = bind(acpt_sock, (sockaddr*)&acpt_adr, sizeof(acpt_adr));
    if (rtn == -1) {
        std::cout << "bind error" << std::endl;
        return 0;
    }

    listen(acpt_sock, 5);

    sockaddr_in recv_adr;
    std::memset(&recv_adr, 0, sizeof(recv_adr));
    socklen_t recv_sz = sizeof(recv_adr);

    int recv_sock = accept(acpt_sock, (sockaddr*)&recv_adr, &recv_sz);

    char buf[BUF_SIZE] = { 0 };
    int str_sz = 0;
    while (true) {
        str_sz = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK | MSG_DONTWAIT);
        if (str_sz > 0) {
            break;
        }
    }

    buf[str_sz] = 0;
    std::cout << "Buffering " << str_sz << " bytes: " << buf << std::endl;

    str_sz = recv(recv_sock, buf, sizeof(buf) - 1, 0);
    buf[str_sz] = 0;
    std::cout << "Read again: " << buf << std::endl;

    close(acpt_sock);
    close(recv_sock);

    return 0;
}
