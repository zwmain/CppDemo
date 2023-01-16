# 优于select的epoll

## epoll 理解及应用

select复用方法其实由来已久，因此，利用该技术后，无论如何优化程序性能也无法同时接入上百个客户端（当然，硬件性能不同，差别也很大）。这种select方式并不适合以Web服务器端开发为主流的现代开发环境，所以要学习Linux平台下的epoll。

基于select的I/O复用技术速度慢的原因

第12章曾经实现过基于select的I/O复用服务器端，很容易从代码上分析出不合理的设计，最主要的两点如下。

- 调用select函数后常见的针对所有文件描述符的循环语句。
- 每次调用select函数时都需要向该函数传递监视对象信息。

上述两点可以从第12章示例echo selectserv.c的第45、49行及第54行代码得到确认。调用select 函数后，并不是把发生变化的文件描述符单独集中到一起，而是通过观察作为监视对象的fd_set 变量的变化，找出发生变化的文件描述符（示例echo selectserv.c的第54、56行），因此无法避免针对所有监视对象的循环语句。而且，作为监视对象的fd set变量会发生变化，所以调用select函数前应复制并保存原有信息（参考echo selectserv.c的第45行），并在每次调用select函数时传递新的监视对象信息。

各位认为哪些因素是提高性能的更大障碍？是调用select函数后常见的针对所有文件描述符对象的循环语句？还是每次需要传递的监视对象信息？


只看代码的话很容易认为是循环。但相比于循环语句，更大的障碍是每次传递监视对象信息。因为传递监视对象信息具有如下含义：

"每次调用select函数时向操作系统传递监视对象信息。"

应用程序向操作系统传递数据将对程序造成很大负担，而且无法通过优化代码解决，因此将成为性能上的致命弱点。

“那为何需要把监视对象信息传递给操作系统呢？”

有些函数不需要操作系统的帮助就能完成功能，而有些则必须借助于操作系统。假设各位定义了四则运算相关函数，此时无需操作系统的帮助。但select函数与文件描述符有关，更准确地说，是监视套接字变化的函数。而套接字是由操作系统管理的，所以select函数绝对需要借助于操作系统才能完成功能。select函数的这一缺点可以通过如下方式弥补：

“仅向操作系统传递1次监视对象，监视范围或内容发生变化时只通知发生变化的事项。”

这样就无需每次调用select函数时都向操作系统传递监视对象信息，但前提是操作系统支持这种处理方式（每种操作系统支持的程度和方式存在差异）。Linux的支持方式是epoll，Windows 的支持方式是IOCP。

select也有优点

知道这些内容后，有些人可能对select函数感到失望，但大家应当掌握select函数。本章的epoll 方式只在Linux下提供支持，也就是说，改进的I/O复用模型不具有兼容性。相反，大部分操作系统都支持select函数。只要满足或要求如下两个条件，即使在Linux平台也不应拘泥于epoll。

- 服务器端接入者少。
- 程序应具有兼容性。

实际并不存在适用于所有情况的模型。各位应理解好各种模型的优缺点，并具备合理运用这些模型的能力。

实现epol时必要的函数和结构体

能够克服select函数缺点的epoll函数具有如下优点，这些优点正好与之前的select函数缺点相反。

- 无需编写以监视状态变化为目的的针对所有文件描述符的循环语句。
- 调用对应于select函数的epoll_wait函数时无需每次传递监视对象信息。

下面介绍epoll服务器端实现中需要的3个函数，希望各位结合epoll函数的优点理解这些函数的功能。

- epoll_create：创建保存epoll文件描述符的空间。
- epoll ctl：向空间注册并注销文件描述符。
- epoll_wait：与select函数类似，等待文件描述符发生变化。

select方式中为了保存监视对象文件描述符，直接声明了fd_set变量。但epoll方式下由操作系统负责保存监视对象文件描述符，因此需要向操作系统请求创建保存文件描述符的空间，此时使用的函数就是epoll_create。

此外，为了添加和删除监视对象文件描述符，select方式中需要FD_SET、FD_CLR函数。但在epoll方式中，通过epoll ctl函数请求操作系统完成。最后，select方式下调用select函数等待文件描述符的变化，而epoll中调用epoll_wait函数。还有，select方式中通过fi_set变量查看监视对象的状态变化（事件发生与否），而epoll方式中通过如下结构体epoll_event将发生变化的（发生事件的）文件描述符单独集中到一起。

```c
typedef union epoll_data {
    void* ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

struct epoll_event {
    uint32_t events;
    epoll_data_t data;
};

```

声明足够大的epoll event结构体数组后，传递给epollwait函数时，发生变化的文件描述符信息将被填入该数组。因此，无需像select函数那样针对所有文件描述符进行循环。

以上就是epoll中需要的函数和结构体。实际上，只要有select程序的编写经验，epoll程序的编写就并不难。接下来给出这些函数的详细说明。

epoll_create

epoll是从Linux的2.5.44版内核（操作系统的核心模块）开始引入的，所以使用epoll前需要验证Linux内核版本。但各位使用的Linux内核基本都是2.6以上的版本，所以这部分可以忽略。若有人怀疑自己的Linux版本过低，可以通过如下命令验证：

```shell
cat /proc/sys/kernel/osrelease
```

下面仔细观察epoll_create函数。

```c
#include <sys/epoll.h>
int epoll_create(int size);
// 成功时返回 epoll文件描述符，失败时返回-1。
// size epoll实例的大小。

```

调用epoll_create函数时创建的文件描述符保存空间称为"epoll例程"，但有些情况下名称不同，需要稍加注意。通过参数size传递的值决定epoll例程的大小，但该值只是向操作系统提的建议。换言之，size并非用来决定epoll例程的大小，而仅供操作系统参考。

> 操作系统将完全忽略传递给epoll_create的参数
>
> Linux 2.6.8之后的内核将完全忽略传入 epoll_create 函数的 size 参数，因为内核会根据情况调整 epoll例程的大小。但撰写本书时Linux版本未达到2.6.8，因此无法在忽略size参数的情况下编写程序。

epoll_create函数创建的资源与套接字相同，也由操作系统管理。因此，该函数和创建套接字的情况相同，也会返回文件描述符。也就是说，该函数返回的文件描述符主要用与于区分epoll 例程。需要终止时，与其他文件描述符相同，也要调用close函数。

epoll_ctl

生成epoll例程后，应在其内部注册监视对象文件描述符，此时使用epoll_ctl函数。

```c
#include <sys/epoll.h>
int epoll_ctl(int epfd, int op, int fd, struct epoll_event * event);
// 成功时返回0，失败时返回-1。
// epfd         用于注册监视对象的epoll例程的文件描述符。
// op           用于指定监视对象的添加、删除或更改等操作。
// fd           需要注册的监视对象文件描述符。
// event        监视对象的事件类型。

```

与其他epoll函数相比，该函数多少有些复杂，但通过调用语句就很容易理解。假设按照如下形式调用epoll_ctl函数：

```c
epoll_ctl(A, EPOLL_CTL_ADD, B, C);
```

第二个参数EPOLL_CTL_ADD意味着"添加"，因此上述语句具有如下含义：

"向epoll A中注册文件描述符B，主要目的是监视参数C中的事件。"

再介绍一个调用语句。

```c
epoll_ctl(A, EPOLL_CTL_DEL, B, NULL);
```

上述语句中第二个参数EPOLL_CTL_DEL指"删除"，因此该语句具有如下含义：

"从epoll A中删除文件描述符B。"

从上述调用语句中可以看到，从监视对象中删除时，不需要监视类型（事件信息），因此向第四个参数传递NULL。接下来介绍可以向epoll_ctl第二个参数传递的常量及含义。

- EPOLL_CTLADD：将文件描述符注册到epoll例程。
- EPOLL_CTL_DEL：从epoll例程中删除文件描述符。
- EPOLL_CTLMOD：更改注册的文件描述符的关注事件发生情况。

关于EPOLL CTL MOD常量稍后讲解（即使我不讲大家也自然能明白）。如前所述，向epoll_ctl的第二个参数传递EPOLL_CTL_DEL时，应同时向第四个参数传递NULL。但Linux2.6.9 之前的内核不允许传递NULL。虽然被忽略掉，但也应传递epoll_event结构体变量的地址值（本书示例将传递NULL）。其实这是Bug，但也没必要因此怀疑epoll的功能，因为我们使用的标准函数中也存在Bug。

下面讲解各位不太熟悉的epoll_ctl函数的第四个参数，其类型是之前将过的epoll_event结构体指针。

"啊？不是说epoll_event用于保存发生变化的（发生事件）的文件描述符吗？"

当然！如前所述，epoll_event结构体用于保存发生事件的文件描述符集合。但也可以在epoll 例程中注册文件描述符时，用于注册关注的事件。函数中epoll_event结构体的定义并不显眼，因此通过调用语句说明该结构体在epoll_ctl函数中的应用。

```c
struct epoll_event event;
event.events=EPOLLIN; //发生需要读取数据的情况（事件）时
event.data.fd=sockfd;
epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);

```

上述代码将sockfd注册到epoll例程epfd中，并在需要读取数据的情况下产生相应事件。接下来给出epoll_event的成员events中可以保存的常量及所指的事件类型。

- EPOLLIN：需要读取数据的情况。
- EPOLLOUT：输出缓冲为空，可以立即发送数据的情况。
- EPOLLPRI：收到OOB数据的情况。
- EPOLLRDHUP：断开连接或半关闭的情况，这在边缘触发方式下非常有用。
- EPOLLERR：发生错误的情况。
- EPOLLET：以边缘触发的方式得到事件通知。
- EPOLLONESHOT：发生一次事件后，相应文件描述符不再收到事件通知。因此需要向epoll_ctl函数的第二个参数传递EPOLL_CTL_MOD，再次设置事件。

可以通过位或运算同时传递多个上述参数。关于“边缘触发”稍后将单独讲解，目前只需记住EPOLLIN即可。

epoll_wait

最后介绍与select函数对应的epoll_wait函数，epoll相关函数中默认最后调用该函数。

```c
#include <sys/epoll.h>
int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);
// 成功时返回发生事件的文件描述符数，失败时返回-1。
// epfd         表示事件发生监视范围的epoll例程的文件描述符。
// events       保存发生事件的文件描述符集合的结构体地址值。
// maxevents    第二个参数中可以保存的最大事件数。
// timeout      以1/1000秒为单位的等待时间，传递-1时，一直等待直到发生事件。

```

该函数的调用方式如下。需要注意的是，第二个参数所指缓冲需要动态分配。

```c++
epoll_event* ep_events = new epoll_event[EPOLL_SIZE];
int event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
// ...
delete[] ep_events;

```

调用函数后，返回发生事件的文件描述符数，同时在第二个参数指向的缓冲中保存发生事件的文件描述符集合。因此，无需像select那样插入针对所有文件描述符的循环。

基于epoll的回声服务器端

以上就是基于epoll技术实现服务器端的所有理论说明，接下来给出基于epoll的回声服务器端示例。我通过更改第12章的echo_selectserv.c实现了该示例。当然，从头开始写也与下面给出的内容类似。但通过更改select示例理解二者差异将更有利于学习。

```c++
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 1024;
constexpr size_t EPOLL_SIZE = 64;

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
    epEvt.events = EPOLLIN; // 类型指定为EPOLLIN
    epEvt.data.fd = servSock; // 目标套接字为servSock
    // 注册epoll事件
    epoll_ctl(epFd, EPOLL_CTL_ADD, servSock, &epEvt);

    char buf[BUF_SIZE] = { 0 };
    // 动态创建epoll_event数组，保存发生变化的事件
    epoll_event* evtArr = new epoll_event[EPOLL_SIZE];

    while (true) {
        // 返回事件数量
        int epCnt = epoll_wait(epFd, evtArr, EPOLL_SIZE, -1);
        if (epCnt == -1) {
            std::cout << "epoll_wait 错误" << std::endl;
            break;
        }

        // 遍历已触发的事件
        for (int i = 0; i < epCnt; ++i) {
            int sockId = evtArr[i].data.fd;
            // 如果变化的事件套接字等于服务端套接字，说明有新的请求
            if (sockId == servSock) {
                sockaddr_in clntAdr;
                socklen_t szAdr = sizeof(clntAdr);
                int clntSock = accept(servSock, (sockaddr*)&clntAdr, &szAdr);
                // 注册接收的套接字到epoll
                epEvt.events = EPOLLIN;
                epEvt.data.fd = clntSock;
                epoll_ctl(epFd, EPOLL_CTL_ADD, clntSock, &epEvt);

                std::string adrStr = inet_ntoa(clntAdr.sin_addr);
                std::cout << "接收" << adrStr << "的请求：" << clntSock << std::endl;
            } else {
                int strLen = read(sockId, buf, BUF_SIZE);
                if (strLen == 0) {
                    // 说明客户端断开连接
                    epoll_ctl(epFd, EPOLL_CTL_DEL, sockId, nullptr);
                    close(sockId);
                    std::cout << "客户端：" << sockId << "断开连接" << std::endl;
                } else {
                    buf[strLen] = { 0 };
                    std::cout << "接收客户端" << sockId << "的数据：" << buf << std::endl;
                    write(sockId, buf, strLen);
                }
            }
        }
    }

    close(epFd);
    close(servSock);

    delete[] evtArr;

    return 0;
}


```

之前解释过关键代码，而且程序结构与select方式没有区别，故省略代码说明。如果有些地方难以理解，说明未掌握本章之前的内容和select模型，建议复习。结合我的说明和select示例理解上述代码也是一种很好的学习方式。上述示例可以结合任意回声客户端运行，而且运行结果与其他回声服务器端/客户端程序没有差别，故省略。











