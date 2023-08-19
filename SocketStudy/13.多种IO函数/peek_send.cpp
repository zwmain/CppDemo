#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <IP> <PORT>" << std::endl;
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in send_adr;
    std::memset(&send_adr, 0, sizeof(send_adr));
    send_adr.sin_family = AF_INET;
    send_adr.sin_addr.s_addr = inet_addr(argv[1]);
    send_adr.sin_port = htons(std::atoi(argv[2]));

    int rtn = connect(sock, (sockaddr*)&send_adr, sizeof(send_adr));
    if (rtn == -1) {
        std::cout << "connect error" << std::endl;
        return 0;
    }

    std::string msg = "123";
    write(sock, msg.c_str(), msg.size());
    close(sock);

    return 0;
}
