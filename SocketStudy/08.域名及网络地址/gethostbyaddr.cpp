#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <linux/in.h>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " IP" << std::endl;
        return 0;
    }

    sockaddr_in adr;
    std::memset(&adr, 0, sizeof(adr));
    adr.sin_addr.s_addr = inet_addr(argv[1]);
    hostent* host = gethostbyaddr(&adr.sin_addr, sizeof(adr.sin_addr), AF_INET);
    if (host == nullptr) {
        std::cout << "gethostbyaddr 错误" << std::endl;
        return 0;
    }

    std::cout << "官方名称：" << host->h_name << std::endl;
    for (int i = 0; host->h_aliases[i] != nullptr; ++i) {
        std::cout << "别名" << i + 1 << "：" << host->h_aliases[i] << std::endl;
    }

    std::cout << "IP类型：" << (host->h_addrtype == AF_INET ? "IPv4" : "IPv6") << std::endl;
    for (int i = 0; host->h_addr_list[i] != nullptr; ++i) {
        char buf[30] = { 0 };
        inet_ntop(AF_INET, host->h_addr_list[i], buf, 30);
        std::cout << "IP" << i + 1 << "：" << buf << std::endl;
    }

    return 0;
}

