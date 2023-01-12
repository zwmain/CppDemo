#include <arpa/inet.h>
#include <iostream>
#include <linux/in.h>
#include <string>
#include <sys/endian.h>

int main(int argc, char* argv[])
{
    sockaddr_in addr1;
    addr1.sin_addr.s_addr = htonl(0x1020304);

    std::string str1 = inet_ntoa(addr1.sin_addr);
    std::cout << "点分十进制地址1：" << str1 << std::endl;

    sockaddr_in addr2;
    addr2.sin_addr.s_addr = htonl(0x1010101);

    std::string str2 = inet_ntoa(addr2.sin_addr);
    std::cout << "点分十进制地址2：" << str2 << std::endl;

    return 0;
}

