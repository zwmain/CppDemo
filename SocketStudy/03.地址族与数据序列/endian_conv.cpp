#include <arpa/inet.h>
#include <iostream>

int main(int argc, char* argv[])
{
    unsigned short hostPort = 0x1234;
    unsigned short netPort = htons(hostPort);
    unsigned long hostAddr = 0x12345678;
    unsigned long netAddr = htonl(hostAddr);

    std::cout << "Host ordered port: " << hostPort << std::endl;
    std::cout << "Network ordered port: " << netPort << std::endl;
    std::cout << "Host ordered address: " << hostAddr << std::endl;
    std::cout << "Network ordered address: " << netAddr << std::endl;

    return 0;
}

