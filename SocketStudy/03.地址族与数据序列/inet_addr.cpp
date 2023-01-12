#include <arpa/inet.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    std::string addr1 = "1.2.3.4";
    unsigned long int convAddr = inet_addr(addr1.c_str());
    if (convAddr == INADDR_NONE) {
        std::cout << "Address1 error!" << std::endl;
    } else {
        std::cout << "网络字节序的整数地址1：" << convAddr << std::endl;
    }

    std::string addr2 = "1.2.3.256";
    convAddr = inet_addr(addr2.c_str());
    if (convAddr == INADDR_NONE) {
        std::cout << "Address2 error!" << std::endl;
    } else {
        std::cout << "网络字节序的整数地址2：" << convAddr << std::endl;
    }

    return 0;
}

