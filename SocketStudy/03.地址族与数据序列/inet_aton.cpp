#include <arpa/inet.h>
#include <iostream>
#include <linux/in.h>
#include <string>

int main(int argc, char* argv[])
{
    std::string addr = "127.232.124.79";
    sockaddr_in addrInet;
    int stu = inet_aton(addr.c_str(), &addrInet.sin_addr);
    if (stu) {
        std::cout << "网络字节序的整数地址：" << addrInet.sin_addr.s_addr << std::endl;
    } else {
        std::cout << "inet_aton 错误" << std::endl;
    }

    return 0;
}

