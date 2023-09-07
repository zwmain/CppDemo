#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <linux/in.h>
#include <sys/endian.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef FILEPATH
#define FILEPATH "../news.txt"
#endif

constexpr int BUF_SIZE = 30;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <Boardcast IP> <PORT>" << std::endl;
        return 0;
    }

    int send_socket = socket(PF_INET, SOCK_DGRAM, 0);
    sockaddr_in broad_adr;
    std::memset(&broad_adr, 0, sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = inet_addr(argv[1]);
    broad_adr.sin_port = htons(std::atoi(argv[2]));

    int so_brd = 1;
    setsockopt(send_socket, SOL_SOCKET, SO_BROADCAST, &so_brd, sizeof(so_brd));

    std::ifstream fi { FILEPATH };
    while (!fi.eof()) {
        std::string msg;
        fi >> msg;
        sendto(send_socket, msg.c_str(), msg.size(), 0, (sockaddr*)&broad_adr, sizeof(broad_adr));
        std::cout << msg << std::endl;
        sleep(2);
    }

    close(send_socket);

    return 0;
}

