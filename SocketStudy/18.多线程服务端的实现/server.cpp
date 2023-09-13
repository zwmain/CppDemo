#include <ThreadPool.hpp>
#include <arpa/inet.h>
#include <csignal>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 4;
constexpr size_t EPOLL_SIZE = 64;
bool isRunning = true;

// 封装函数，设置套接字非阻塞
void setNoBlockingMode(int fd);
void dealWithRequest(const int servSock, const int curSock, const int epFd);
void handleSig(int sig);

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

    // 初始化地址信息
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

    // 创建epoll
    int epFd = epoll_create(EPOLL_SIZE);
    if (epFd == -1) {
        close(servSock);
        std::cout << "epoll_create 错误" << std::endl;
        return 0;
    }

    // 初始化事件类型
    epoll_event epEvt;
    epEvt.events = EPOLLIN | EPOLLET; // 类型指定为EPOLLIN
    epEvt.data.fd = servSock; // 目标套接字为servSock
    // 注册epoll事件
    epoll_ctl(epFd, EPOLL_CTL_ADD, servSock, &epEvt);

    char buf[BUF_SIZE] = { 0 };
    // 动态创建epoll_event数组，保存发生变化的事件
    epoll_event* evtArr = new epoll_event[EPOLL_SIZE];

    zwn::ThreadPool globalPool;
    globalPool.start();
    signal(SIGINT, handleSig);

    size_t cntWait = 1;
    while (isRunning) {
        // 返回事件数量
        int epCnt = epoll_wait(epFd, evtArr, EPOLL_SIZE, 3000);
        if (epCnt == -1) {
            std::cout << "epoll_wait 错误" << std::endl;
            break;
        }

        std::cout << "epoll_wait次数：" << cntWait++ << std::endl;
        // 遍历已触发的事件
        for (int i = 0; i < epCnt; ++i) {
            int sockId = evtArr[i].data.fd;
            // dealWithRequest(servSock, sockId, epFd);
            globalPool.addTask(dealWithRequest, servSock, sockId, epFd);
        }
    }

    std::cout << "已退出主循环" << std::endl;

    globalPool.close();

    close(epFd);
    close(servSock);

    delete[] evtArr;

    return 0;
}

void setNoBlockingMode(int fd)
{
    int flag = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

void dealWithRequest(const int servSock, const int curSock, const int epFd)
{
    // 如果变化的事件套接字等于服务端套接字，说明有新的请求
    if (curSock == servSock) {
        sockaddr_in clntAdr;
        socklen_t szAdr = sizeof(clntAdr);
        int clntSock = accept(servSock, (sockaddr*)&clntAdr, &szAdr);
        // 将接收的套接字设置为非阻塞
        setNoBlockingMode(clntSock);
        // 注册接收的套接字到epoll
        epoll_event epEvt;
        epEvt.events = EPOLLIN | EPOLLET;
        epEvt.data.fd = clntSock;
        epoll_ctl(epFd, EPOLL_CTL_ADD, clntSock, &epEvt);

        std::string adrStr = inet_ntoa(clntAdr.sin_addr);
        std::cout << "接收" << adrStr << "的请求：" << clntSock << std::endl;
    } else {
        // 读取循环
        while (true) {
            char buf[BUF_SIZE] = { 0 };
            int strLen = read(curSock, buf, BUF_SIZE);
            if (strLen == 0) {
                // 说明客户端断开连接
                epoll_ctl(epFd, EPOLL_CTL_DEL, curSock, nullptr);
                close(curSock);
                std::cout << "客户端：" << curSock << "断开连接" << std::endl;
                break;
            } else if (strLen < 0) {
                if (errno == EAGAIN) {
                    // 发生错误且错误值为EAGAIN时，说明读取完毕
                    std::cout << "套接字：" << curSock << "读取完毕" << std::endl;
                } else {
                    std::cout << "套接字：" << curSock << "读取错误" << std::endl;
                }
                break;
            } else {
                buf[strLen] = { 0 };
                std::cout << "接收客户端" << curSock << "的数据：" << buf << std::endl;
                write(curSock, buf, strLen);
            }
        }
    }
}

void handleSig(int sig)
{
    if (sig == SIGINT) {
        std::cout << "程序正在终止" << std::endl;
        isRunning = false;
    }
}

