#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

constexpr int TTL = 64;
constexpr int BUF_SIZE = 30;

#ifndef FILEPATH
#define FILEPATH "../news.txt"
#endif

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << ' ';
    }
    std::cout << std::endl;
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <GroupIP> <PORT>" << std::endl;
        return 0;
    }

    int send_sock = socket(PF_INET, SOCK_DGRAM, 0);

    // 初始化目标地址，和普通的目标地址没有区别，只是传进来的参数是一个多播地址而已
    sockaddr_in mul_adr;
    std::memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(std::atoi(argv[2]));

    // 设置套接字TTL
    int time_live = TTL;
    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));

    std::ifstream fi { FILEPATH };
    while (!fi.eof()) {
        std::string msg;
        fi >> msg;
        sendto(send_sock, msg.c_str(), msg.size(), 0, (sockaddr*)&mul_adr, sizeof(mul_adr));
        std::cout << msg << std::endl;
        sleep(2);
    }
    close(send_sock);
    return 0;
}
