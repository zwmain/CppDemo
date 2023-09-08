#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
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
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << "<Self IP> <Boardcast IP> <PORT>" << std::endl;
        return 0;
    }

    int send_socket = socket(PF_INET, SOCK_DGRAM, 0);

    sockaddr_in self_adr;
    std::memset(&self_adr, 0, sizeof(self_adr));
    self_adr.sin_family = AF_INET;
    self_adr.sin_addr.s_addr = inet_addr(argv[1]);
    self_adr.sin_port = 0;
    int res = bind(send_socket, (sockaddr*)&self_adr, sizeof(self_adr));
    if (res == -1) {
        std::cout << "bind error";
    }

    sockaddr_in broad_adr;
    std::memset(&broad_adr, 0, sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = inet_addr(argv[2]);
    broad_adr.sin_port = htons(std::atoi(argv[3]));

    int so_brd = 1;
    int rtn = setsockopt(send_socket, SOL_SOCKET, SO_BROADCAST, &so_brd, sizeof(so_brd));
    if (rtn == -1) {
        std::cout << "setsockopt error" << std::endl;
        return 0;
    }

    std::ifstream fi { FILEPATH };
    while (!fi.eof()) {
        std::string msg;
        fi >> msg;
        int s = sendto(send_socket, msg.c_str(), msg.size(), 0, (sockaddr*)&broad_adr, sizeof(broad_adr));
        std::cout << s << ":" << msg << std::endl;
        sleep(2);
    }

    close(send_socket);

    return 0;
}

