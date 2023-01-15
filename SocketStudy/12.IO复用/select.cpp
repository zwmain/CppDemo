#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 1024;

int main(int argc, char* argv[])
{
    fd_set reads;
    // 初始化reads
    FD_ZERO(&reads);
    // 监视描述符0，0代表标准输入，控制台输入
    FD_SET(0, &reads);

    timeval timeout;
    // 不能在这里设置时间
    // 因为调用select函数后，结构体timeval的成员tv_sec和tv_usec的值将被替换为超时前剩余时间。
    // 因此，调用select函数前，每次都需要初始化timeval结构体变量。
    // timeout.tv_sec = 5;
    // timeout.tv_usec = 0;

    char buf[BUF_SIZE] = { 0 };

    while (true) {
        // 将准备好的fd_set变量reads的内容复制到temps变量，因为之前讲过，调用select 函数后，除发生变化的文件描述符对应位外，剩下的所有位将初始化为0。
        // 因此，为了记住初始值，必须经过这种复制过程。
        // 这是使用select函数的通用方法，希望各位牢记。
        fd_set temps = reads;
        // 每次都需要初始化timeval结构体变量
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        // 第一个参数是1，因为监视的最后一个描述符是0，+1就是1
        // 第二个是reads的备份，后两个没有要监视的，所以传空，最后传入超时
        int res = select(1, &temps, nullptr, nullptr, &timeout);
        if (res == -1) {
            // 返回-1，说明出现错误
            std::cout << "select 错误" << std::endl;
            break;
        } else if (res == 0) {
            // 返回0，说明超时
            std::cout << "超时" << std::endl;
        } else {
            // 大于0代表发生事件的描述符个数，这里只有一个，所以直接判断
            if (FD_ISSET(0, &temps)) {
                int strLen = read(0, buf, BUF_SIZE);
                buf[strLen] = 0;
                std::cout << "select 事件数据：" << buf << std::endl;
            }
        }
    }

    return 0;
}

