#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

constexpr int BUF_SIZE = 1024;

#ifndef DATA_PATH
#define DATA_PATH "../server.cpp"
#endif

std::vector<std::string> getData();

int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <IP> <PORT> <NUM>" << std::endl;
        return 0;
    }

    int clntSock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in servAdr;
    std::memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);
    servAdr.sin_port = htons(std::atoi(argv[2]));

    int rtn = connect(clntSock, (sockaddr*)&servAdr, sizeof(servAdr));
    if (rtn == -1) {
        close(clntSock);
        std::cout << "connect error" << std::endl;
        return 0;
    }

    auto dataArr = getData();
    dataArr.push_back("Client End");

    const int NUM = std::atoi(argv[3]);
    char buf[BUF_SIZE] = { 0 };
    for (int i = 0; i < NUM; ++i) {
        int j = i % dataArr.size();
        auto& msg = dataArr[j];
        write(clntSock, msg.c_str(), msg.size());
        int l = read(clntSock, buf, BUF_SIZE - 1);
        if (l <= 0) {
            continue;
        }
        buf[l] = 0;
        std::cout << buf << std::endl;
    }

    close(clntSock);

    return 0;
}

std::vector<std::string> getData()
{
    std::vector<std::string> res;
    std::ifstream fi { DATA_PATH };
    if (!fi.is_open()) {
        std::cout << "read file error: " << DATA_PATH << std::endl;
        return res;
    }
    while (!fi.eof()) {
        std::string l;
        std::getline(fi, l);
        res.push_back(std::move(l));
    }
    std::cout << "数据共" << res.size() << "行" << std::endl;
    return res;
}
