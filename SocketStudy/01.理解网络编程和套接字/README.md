# 理解网络编程和套接字

## 理解网络编程和套接字

**网络编程和套接字概要**

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

**编写"Hello World!"服务端**

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

Windows套接字（以下简称Winsock）大部分是参考BSD系列UNIX套接字设计的，所以很多地方都跟Linux套接字类似。因此，只需要更改Linux环境下编好的一部分网络程序内容，就能在Windows平台下运行。
本书也会同时讲解Linux和Windows两大平台，这不会给大家增加负担，反而会减轻压力。

**同时学习Linux和Windows的原因**

大多数项目都在Linux系列的操作系统下开发服务器端，而多数客户端是在Windows平台下开发的。不仅如此，有时应用程序还需要在两个平台之间相互切换。
因此，学习套接字编程的过程中，有必要兼顾Windows和Limux两大平台。另外，这两大平台下的套接字编程非常类似，如果把其中相似的部分放在一起讲解，将大大提高学习效率。
这会不会增加学习负担？一点也不。只要理解好其中一个平台下的网络编程方法，就很容易通过分析差异掌握另一平台。

**为 Windows 套接字编程设置头文件和库**

- 导入头文件WinSock2.h
- 链接ws2_32.lib库

**Winsock 的初始化**

进行Winsock编程时，首先必须调用WSAStartup函数，设置程序中用到的Winsock版本，并初始化相应版本的库。

```c
#include <winsock2.h>
int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
// 成功时返回0，失败时返回非零的错误代码值。
// wVersionRequested   程序员要用的Winsock版本信息。
// lpWSAData           WSADATA结构体变量的地址值。

```

有必要给出上述两个参数的详细说明。先说第一个，Winsock中存在多个版本，应准备WORD 类型的（WORD是通过typedef声明定义的unsigned short类型）套接字版本信息，并传递给该函数的第一个参数wVersionRequested。
若版本为1.2，则其中1是主版本号，2是副版本号，应传递0x0201。

如前所述，高8位为副版本号，低8位为主版本号，以此进行传递。本书主要使用2.2版本，故应传递0x0202。
不过，以字节为单位手动构造版本信息有些麻烦，借助MAKEWORD宏函数则能轻松构建WORD型版本信息。

- MAKEWORD（1，2） //主版本为1，副版本为2，返回0x0201。
- MAKEWORD（2，2） //主版本为2，副版本为2，返回0x0202。


接下来讲解第二个参数lpWSADATA，此参数中需传入WSADATA型结构体变量地址（LPWSADATA是WSADATA的指针类型）。
调用完函数后，相应参数中将填充已初始化的库信息。虽无特殊含义，但为了调用函数，必须传递WSADATA结构体变量地址。
下面给出WSAStartup函数调用过程，这段代码几乎已成为Winsock编程的公式。

```c
int main(int argc, char* argv[])
{
    WSADATA wsaDAta;
    // ...
    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0) {
        ErrorHandling("WSAStartup() error!");
    }
    // ...
    return 0;
}

```

前面已经介绍了Winsock相关库的初始化方法，接下来讲解如何注销该库——利用下面给出的函数。

```c
#include <winsock2.h>
int wSACleanup(void);

```

成功时返回 0，失败时返回 SOCKET_ERROR。

调用该函数时，Winsock相关库将归还Windows操作系统，无法再调用Winsock相关函数。从原则上讲，无需再使用Winsock函数时才调用该函数，但通常都在程序结束之前调用。

## 基于Windows的套接字相关函数及示例

本节介绍的Winsock函数与之前的Linux套接字相关函数相对应。既然只是介绍，就不做详细说明了，目的只在于让各位体会基于Linux和Windows的套接字函数之间的相似性。

**基于Windows的套接字相关函数**

首先介绍的函数与Linux下的socket函数提供相同功能。稍后讲解返回值类型SOCKET。

```c
#include <winsock2.h>
SOCKET socket(int af, int type, int protocol);
// 成功时返回套接字句柄，失败时返回INVALID_SOCKET。

```

下列函数与Linux的bind函数相同，调用其分配IP地址和端口号。

```c
#include<winsock2.h>
int bind(SOCKET s, const struct sockaddr *name, int namelen);
// 成功时返回0，失败时返回SOCKETERROR。

```

下列函数与Linux的listen函数相同，调用其使套接字可接收客户端连接

```c
#include <winsock2.h>
intlisten(SOCKET s, int backlog);
// 成功时返回0，失败时返回SOCKETERROR。

```

下列函数与Linux的accept函数相同，调用其受理客户端连接请求。

```c
#include<winsock2.h>
SOCKET accept(SOCKET s, struct sockaddr *addr, int *addrlen);
// 成功时返回套接字句柄，失败时返回INVALID_SOCKET。

```

下列函数与Linux的connect函数相同，调用其从客户端发送连接请求。

```c
#include<winsock2.h>
int connect(SOCKET s, const struct sockaddr *name, int namelen);
// 成功时返回0，失败时返回SOCKETERROR。

```

最后这个函数在关闭套接字时调用。Linux中，关闭文件和套接字时都会调用close函数;而Windows中有专门用来关闭套接字的函数。

```c
#include<winsock2.h>
int closesocket(SOCKET s);
// 成功时返回0，失败时返回SOCKETERROR。

```

以上就是基于Windows的套接字相关函数，虽然返回值和参数与Linux函数有所区别，但具有相同功能的函数名是一样的。正是这些特点使跨越两大操作系统平台的网络编程更加简单。

**Windows中的文件句柄和套接字句柄**

Linux内部也将套接字当作文件，因此，不管创建文件还是套接字都返回文件描述符。之前也通过示例介绍了文件描述符返回及编号的过程。
Windows中通过调用系统函数创建文件时，返回“句柄”（handle），换言之，Windows中的句柄相当于Linux中的文件描述符。
只不过Windows 中要区分文件句柄和套接字句柄。虽然都称为"句柄"，但不像Linux那样完全一致。文件句柄相关函数与套接字句柄相关函数是有区别的，这一点不同于Linux文件描述符。

既然对句柄有了一定理解，接下来再观察基于Windows的套接字相关函数，这将加深各位对SOCKET类型的参数和返回值的理解。
的确！这就是为了保存套接字句柄整型值的新数据类型，它由typedef声明定义。回顾socket、listen和accept等套接字相关函数，则更能体会到与Linux中套接字相关函数的相似性。

有些程序员可能会问："既然Winsock是以UNIX、Linux系列的BSD套接字为原型设计的，为什么不照搬过来，而是存在一定差异呢？
"有人认为这是微软为了防止UNIX、Linux服务器端直接移植到Windows而故意为之。从网络程序移植性角度上看，这也是可以理解的。
但我有不同意见。从本质上说，两种操作系统内核结构上存在巨大差异，而依赖于操作系统的代码实现风格也不尽相同，连Windows程序员给变量命名的方式也不同于Linux程序员。
从各方面考虑，保持这种差异性就显得比较自然。因此我个人认为，Windows套接字与BSD系列的套接字编程方式有所不同是为了保持这种自然差异性。

**创建基于Windows的服务器端和客户端**

接下来将之前基于Linux的服务器端与客户端示例转化到Windows平台。
目前想完全理解这些代码有些困难，我们只需验证套接字相关函数的调用过程、套接字库的初始化与注销过程即可。先介绍服务器端示例。

```c++
#include <WinSock2.h>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " port" << std::endl;
        return 0;
    }

    // 初始化库
    WSADATA wsaData;
    int stu = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (stu != 0) {
        std::cout << "WSAStartup 错误：" << stu << std::endl;
        return 0;
    }

    // 创建socket
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, 0);
    if (servSock == INVALID_SOCKET) {
        std::cout << "socket 错误" << std::endl;
        return 0;
    }

    // 初始化服务器地址信息
    sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(std::atoi(argv[1]));

    // 调用bind函数绑定地址信息
    stu = bind(servSock, (sockaddr*)&servAddr, sizeof(servAddr));
    if (stu == SOCKET_ERROR) {
        closesocket(servSock);
        std::cout << "bind 错误" << std::endl;
        return 0;
    }

    // 调用listen函数，进入监听状态
    stu = listen(servSock, 5);
    if (stu == SOCKET_ERROR) {
        closesocket(servSock);
        std::cout << "listen 错误" << std::endl;
        return 0;
    }

    sockaddr_in clntAddr;
    int clntAddrSize = sizeof(clntAddr);
    // 接收请求
    SOCKET clntSock = accept(servSock, (sockaddr*)&clntAddr, &clntAddrSize);
    if (clntSock == INVALID_SOCKET) {
        closesocket(servSock);
        std::cout << "accept 错误" << std::endl;
        return 0;
    }

    std::string msg = "Hello World!";
    send(clntSock, msg.c_str(), msg.size(), 0);

    closesocket(clntSock);
    closesocket(servSock);

    // 清理库
    WSACleanup();

    return 0;
}

```

可以看出，除了Winsock库的初始化和注销相关代码、数据类型信息外，其余部分与Linux环境下的示例并无区别。
希望各位阅读这部分代码时与之前的Linux服务器端进行逐行比较。接下来介绍与此示例同步的客户端代码。

```c++
#include <WinSock2.h>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " IP port" << std::endl;
        return 0;
    }

    // 初始化库
    WSADATA wsaData;
    int stu = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (stu != 0) {
        std::cout << "WSAStartup 错误：" << stu << std::endl;
        return 0;
    }

    // 创建socket
    SOCKET clntSock = socket(PF_INET, SOCK_STREAM, 0);
    if (clntSock == INVALID_SOCKET) {
        std::cout << "socket 错误" << std::endl;
        return 0;
    }

    // 初始化服务端地址信息
    sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(std::atoi(argv[2]));

    // 连接到服务端
    stu = connect(clntSock, (sockaddr*)&servAddr, sizeof(servAddr));
    if (stu == SOCKET_ERROR) {
        closesocket(clntSock);
        std::cout << "connect 错误" << std::endl;
        return 0;
    }

    char msg[30] = { 0 };
    int strLen = recv(clntSock, msg, sizeof(msg) - 1, 0);
    if (strLen == -1) {
        closesocket(clntSock);
        std::cout << "recv 错误" << std::endl;
        return 0;
    }

    std::cout << "接收：" << msg << std::endl;
    closesocket(clntSock);

    WSACleanup();

    return 0;
}

```

**基于Windows的I/O函数**

Linux中套接字也是文件，因而可以通过文件I/O函数read和write进行数据传输。
而Windows中则有些不同Windows严格区分文件I/0函数和套接字I/0函数。下面介绍Winsock数据传输函数。

```c
#include<winsock2.h>
int send(SOCKET s, const char *buf, int len, int flags);
// 成功时返回传输字节数，失败时返回SOCKETERROR。
// s        表示数据传输对象连接的套接字句柄值。
// buf      保存待传输数据的缓冲地址值。
// len      要传输的字节数。
// flags    传输数据时用到的多种选项信息。

```

此函数与Linux的write函数相比，只是多出了最后的flags参数。后续章节中将给出该参数的详细说明,在此之前只需传递0,表示不设置任何选项。
但有一点需要注意,send函数并非Windows独有。Linux中也有同样的函数，它也来自于BSD套接字。只不过我在Linux相关示例中暂时只使用read、write函数，为了强调Linux环境下文件I/0和套接字I/0相同。
下面介绍与send函数对应的recv函数。

```c
#include<winsock2.h>
int recv(SOCKET s, const char *buf, int len, int flags);
// 成功时返回接收的字节数(收到EOF时为0)，失败时返回SOCKETERROR。
// s        表示数据接收对象连接的套接字句柄值。
// buf      保存接收数据的缓冲地址值。
// len      能够接收的最大字节数。
// flags    接收数据时用到的多种选项信息。

```

我只是在Windows环境下提前介绍了send、recv函数，以后的Linux示例中也会涉及。
请不要误认为Linux中的read、write函数就是对应于Windows的send、recv函数。
另外，之前的程序代码中也给出了send、recv函数调用过程，故不再另外给出相关示例。




