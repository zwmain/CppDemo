# 理解网络编程和套接字

## 理解网络编程和套接字

网络编程和套接字概要

网络编程就是编写程序使两台连网的计算机相互交换数据。这就是全部内容了吗?是的!网络编程要比想象中简单许多。
那么，这两台计算机之间用什么传输数据呢?
首先需要物理连接。如今大部分计算机都已连接到庞大的互联网，因此不用担心这点。
在此基础上，只需考虑如何编写数据传输软件。但实际上这也不用愁，因为操作系统会提供名为“套接字”(socket)的部件。
套接字是网络数据传输用的软件设备。
即使对网络数据传输原理不太熟悉，我们也能通过套接字完成数据传输因此，网络编程又称为套接字编程。
那为什么要用“套接字”这个词呢?
我们把插头插到插座上就能从电网获得电力供给，同样，为了与远程计算机进行数据传输，需要连接到因特网。
而编程中的“套接字”就是用来连接该网络的工具。
它本身就带有“连接”的含义，如果将其引申，则还可以表示两台计算机之间的网络连接。

套接字大致分为两种，其中，先要讨论的TCP套接字可以比喻成电话机。
实际上，电话机也是通过固定电话网(telephone network)完成语音数据交换的。
因此，我们熟悉的固定电话与套接字实际并无太大区别。下面利用电话机讲解套接字的创建及使用方法。

电话机可以同时用来拨打或接听，但对套接字而言，拨打和接听是有区别的。我们先讨论用于接听的套接字创建过程。

接打电话需要电话机，有了电话机才能安装电话，接下来，我们就准备一部漂亮的电话机。
下列函数创建的就是相当于电话机的套接字。

```c
#include<sys/socket.h>
int socket(int domain, int type, int protocol);
// 成功时返回文件描述符，失败时返回-1。

```

上述函数及本章涉及的其他函数的详细说明将在以后章节中逐一给出，现在只需掌握“原来是由socket函数生成套接字的”就足够了。
另外，我们只需购买机器，剩下的安装和分配电话号码等工作都由电信局的工作人员完成。
而套接字需要我们自已安装，这也是套接字编程难点所在但多安装几次就会发现其实不难。
准备好电话机后要考虑分配电话号码的问题，这样别人才能联系到自己。

套接字同样如此。就像给电话机分配电话号码一样(虽然不是真的把电话号码给了电话机)，利用以下函数给创建好的套接字分配地址信息(IP地址和端口号)。

```c
#include<sys/socket.h>
int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
// 成功时返回0，失败时返回-1。

```

调用bind函数给套接字分配地址后，就基本完成了接电话的所有准备工作。接下来需要连接电话线并等待来电。

一连接电话线，电话机就转为可接听状态，这时其他人可以拨打电话请求连接到该机。
同样，需要把套接字转化成可接收连接的状态。

```c
#include <sys/socket.h>
int listen(int sockfd, int backlog);
// 成功时返回0，失败时返回-1。

```

连接好电话线后，如果有人拨打电话就会响铃，拿起话筒才能接听电话。

拿起话筒意味着接收了对方的连接请求。
套接字同样如此，如果有人为了完成数据传输而请求连接，就需要调用以下函数进行受理。

```c
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
// 成功时返回文件描述符，失败时返回-1。

```

网络编程中接受连接请求的套接字创建过程可整理如下：

- 第一步：调用socket函数创建套接字
- 第二步：调用bind函数分配IP地址和端口号。
- 第三步：调用listen函数转为可接收请求状态。
- 第四步：调用accept函数受理连接请求

记住并掌握这些步骤就相当于为套接字编程勾勒好了轮廓，后续章节会为此轮廓着色。

编写"Hello World!"服务端

服务器端(server)是能够受理连接请求的程序。
下面构建服务器端以验证之前提到的函数调用过程，该服务器端收到连接请求后向请求者返回“Hello world!”答复。
除各种函数的调用顺序外，我们还未涉及任何实际编程。
因此，阅读代码时请重点关注套接字相关函数的调用过程不必理解全部示例。

```c++
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " port" << std::endl;
        return 0;
    }

    // 调用socket函数创建套接字
    int servSock = socket(PF_INET, SOCK_STREAM, 0);
    if (servSock == -1) {
        std::cout << "套接字创建失败！" << std::endl;
        return 0;
    }

    // 初始化地址信息
    sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(std::atoi(argv[1]));

    // 调用bind函数绑定IP地址和端口号
    int stu = bind(servSock, (sockaddr*)&servAddr, sizeof(servAddr));
    if (stu == -1) {
        close(servSock);
        std::cout << "bind 错误" << std::endl;
        return 0;
    }

    // 调用listen函数进入可接收请求的监听状态
    stu = listen(servSock, 5);
    if (stu == -1) {
        close(servSock);
        std::cout << "listen 错误" << std::endl;
        return 0;
    }

    // 调用accept函数受理连接请求
    sockaddr_in clntAddr;
    socklen_t clntAddrSize = sizeof(clntAddr);
    int clntSock = accept(servSock, (sockaddr*)&clntAddr, &clntAddrSize);
    if (clntSock == -1) {
        close(servSock);
        std::cout << "accept 错误" << std::endl;
        return 0;
    }

    std::string msg="Hello World!";
    write(clntSock,msg.c_str(),msg.size());

    close(clntSock);
    close(servSock);

    return 0;
}



```

编译并运行以上示例，创建等待连接请求的服务器端。目前不必详细分析源代码，只需确认之前4个函数调用过程。
稍后将讲解上述示例中调用的write函数。下面讨论如何编写向服务器端发送连接请求的客户端。

构建打电话套接字

服务器端创建的套接字又称为服务器端套接字或监听(listening)套接字。
接下来介绍的套接字是用于请求连接的客户端套接字。
客户端套接字的创建过程比创建服务器端套接字简单，因此直接进行讲解。

负责打电话（请求连接）的函数，如下所示：

```c
#include<sys/socket.h>
int connect(int sockfd, struct sockaddr *serv_addr, socklen_t addrlen);
// 成功时返回0，失败时返回-1。

```

客户端程序只有“调用socket函数创建套接字”和“调用connect函数向服务器端发送连接请求”这两个步骤，因此比服务器端简单。
下面给出客户端，查看以下两项内容:第一，调用socket函数和connect函数;第二，与服务器端共同运行以收发字符串数据。

```c++
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " IP port" << std::endl;
        return 0;
    }

    // 调用socket创建套接字
    int clntSock = socket(PF_INET, SOCK_STREAM, 0);
    if (clntSock == -1) {
        std::cout << "套接字创建失败！" << std::endl;
        return 0;
    }

    // 生成服务器地址信息
    sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(std::atoi(argv[2]));

    // 连接到服务器
    int stu = connect(clntSock, (sockaddr*)&servAddr, sizeof(servAddr));
    if (stu == -1) {
        close(clntSock);
        std::cout << "connect 错误！" << std::endl;
        return 0;
    }

    char msg[30] = { 0 };
    int strLen = read(clntSock, msg, sizeof(msg) - 1);
    if (strLen == -1) {
        std::cout << "read 错误！" << std::endl;
        return 0;
    }

    std::cout << msg << std::endl;

    close(clntSock);

    return 0;
}



```

这样就编好了服务器端和客户端，相信各位会产生好多疑问(实际上不懂的内容比知道的更多)。
接下来的几章将进行解答，请不要着急。

## 基于Linux的文件操作

对于文件操作，主要使用C++标准库的方法。

这里主要学习`read`和`write`函数。

```c
#include<unistd.h>
ssize_t write(int fd, const void *buf, size_t nbytes);
// 成功时返回写入的字节数，失败时返回-1。
// fd     显示数据传输对象的文件描述符。
// buf    保存要传输数据的缓冲地址值。
// nbytes 要传输数据的字节数。

```

此函数定义中，`size_t`是通过typedef声明的unsigned int类型。
对`ssize_t`来说，`size_t`前面多加的s代表signed，即`ssize_t`是通过typedef声明的signed int类型。

```c
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t nbytes);
// 成功时返回接收的字节数(但遇到文件结尾则返回0)，失败时返回-1。
// fd     显示数据接收对象的文件描述符。
// buf    保存要接收数据的缓冲地址值。
// nbytes 要接收数据的字节数。

```

## 基于Windows的实现

跳过




