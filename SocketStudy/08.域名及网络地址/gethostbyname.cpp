#include <arpa/inet.h>
#include <bits/in_addr.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " hostname" << std::endl;
        return 0;
    }

    hostent* host = gethostbyname(argv[1]);
    if (host == nullptr) {
        std::cout << "gethostbyname 错误" << std::endl;
        return 0;
    }

    std::cout << "官方名称：" << host->h_name << std::endl;
    for (int i = 0; host->h_aliases[i] != nullptr; ++i) {
        std::cout << "别名" << i + 1 << "：" << host->h_aliases[i] << std::endl;
    }
    std::cout << "IP类型：" << (host->h_addrtype == AF_INET ? "IPv4" : "IPv6") << std::endl;
    for (int i = 0; host->h_addr_list[i] != nullptr; ++i) {
        std::cout << "IP" << i + 1 << "：" << inet_ntoa(*(in_addr*)host->h_addr_list[i]);
        std::cout << std::endl;
    }

    return 0;
}

