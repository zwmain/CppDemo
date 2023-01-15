#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 1024;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " port" << std::endl;
        return 0;
    }

    // 创建套接字
    int servSock = socket(PF_INET, SOCK_STREAM, 0);
    if (servSock == -1) {
        std::cout << "socket 错误" << std::endl;
        return 0;
    }
    // 初始化服务端地址信息
    sockaddr_in servAdr;
    std::memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(std::atoi(argv[1]));
    // 绑定地址信息
    int stu = bind(servSock, (sockaddr*)&servAdr, sizeof(servAdr));
    if (stu == -1) {
        close(servSock);
        std::cout << "bind 错误" << std::endl;
        return 0;
    }
    // 进入监听状态
    stu = listen(servSock, 5);
    if (stu == -1) {
        close(servSock);
        std::cout << "listen 错误" << std::endl;
        return 0;
    }

    // 初始化fd_set
    fd_set reads;
    FD_ZERO(&reads);
    FD_SET(servSock, &reads);
    int fdMax = servSock;
    // 生命时间对象
    timeval timeout;

    char buf[BUF_SIZE] = { 0 };

    while (true) {
        fd_set cpyReads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int fdNum = select(fdMax + 1, &cpyReads, nullptr, nullptr, &timeout);
        if (fdNum == -1) {
            std::cout << "select 错误" << std::endl;
            break;
        }
        if (fdNum == 0) {
            std::cout << "超时" << std::endl;
            continue;
        }
        // 有fdNum个套接字产生了事件
        // 从0遍历到最大套接字，寻找变化了的套接字
        for (int sockId = 0; sockId <= fdMax; ++sockId) {
            if (!FD_ISSET(sockId, &cpyReads)) {
                continue;
            }
            if (sockId == servSock) {
                // 如果变化的套接字ID等于当前服务端套接字
                // 说明有新的客户端接入
                sockaddr_in clntAdr;
                socklen_t szAdr = sizeof(clntAdr);
                // 接收客户端请求
                int clntSock = accept(servSock, (sockaddr*)&clntAdr, &szAdr);
                // 监听客户端套接字
                FD_SET(clntSock, &reads);
                // 更新最大套接字序号
                if (fdMax < clntSock) {
                    fdMax = clntSock;
                }
                std::string adrStr = inet_ntoa(clntAdr.sin_addr);
                std::cout << "接收" << adrStr << "的请求：" << clntSock << std::endl;
            } else {
                // 接收对应套接字的数据
                int strLen = read(sockId, buf, BUF_SIZE);
                if (strLen == -1) {
                    // read错误
                    FD_CLR(sockId, &reads);
                    close(sockId);
                    std::cout << "套接字" << sockId << "读取错误" << std::endl;
                } else if (strLen == 0) {
                    // 请求已关闭
                    FD_CLR(sockId, &reads);
                    close(sockId);
                    std::cout << "套接字" << sockId << "已关闭" << std::endl;
                } else {
                    // 接收数据后，写给客户端
                    write(sockId, buf, strLen);
                }
            }
        }
    }

    close(servSock);

    return 0;
}

