# 套接字类型与协议设置

因为涉及套接字编程的基本内容，所以第2章和第3章显得相对枯燥一些。但本章内容是第4章介绍的实际网络编程的基础，希望各位反复精读。

大家已经对套接字的概念有所理解，本章将介绍套接字创建方法及不同套接字的特性。在本章仅需了解创建套接字时调用的socket函数，所以希望大家以放松的心态开始学习。

## 套接字协议及其数据传输特性

“协议”这个词给人的第一印象总是相当困难，我在学生时代也这么想。但各位要慢慢熟悉"协议"，因为它几乎是网络编程的全部内容。首先解释其定义。

**关于协议（Protocol）**

如果相隔很远的两人想展开对话，必须先决定对话方式。如果一方使用电话，那么另一方也只能使用电话，而不是书信。
可以说，电话就是两人对话的协议。协议是对话中使用的通信规则，把上述概念拓展到计算机领域可整理为"计算机间对话必备通信规则"。

简言之，协议就是为了完成数据交换而定好的约定。

创建套接字所用的socket函数已经在第1章中简单介绍过。但为了完全理解该函数，此处将再次展开讨论，本章的主要目的也在于此。

```c
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
// 成功时返回文件描述符，失败时返回-1。
// domain       套接字中使用的协议族(Protocol Family)信息。
// type         套接字数据传输类型信息。
// protocol     计算机间通信中使用的协议信息。

```

第1章并未提及该函数的参数，但它们对创建套接字来说是不可或缺的。下面给出详细说明。

**协议族(Protocol Family)**

奶油意大利面和番茄酱意大利面均属于意大利面的一种，与之类似，套接字通信中的协议也具有一些分类。
通过socket函数的第一个参数传递套接字中使用的协议分类信息。此协议分类信息称为协议族，可分成如下几类。

| 名称     | 协议族     |
| ---      | ---        |
| PF_INET    | IPv4互联网协议族     |
| PF_INET6   | IPv6互联网协议族     |
| PF_LOCAL   | 本地通信的UNIX协议族 |
| PF_PACKET  | 底层套接字的协议族   |
| PF_IPX     | IPX Novell协议族     |

本书将着重讲解表中PFINET对应的IPV4互联网协议族。其他协议族并不常用或尚未普及，因此本书将重点放在PF_INET协议族上。
另外，套接字中实际采用的最终协议信息是通过socket函数的第三个参数传递的。在指定的协议族范围内通过第一个参数决定第三个参数。

**套接字类型（Type）**

套接字类型指的是套接字的数据传输方式，通过socket函数的第二个参数传递，只有这样才能决定创建的套接字的数据传输方式。
这种说法可能会使各位感到疑惑。已通过第一个参数传递了协议族信息，还要决定数据传输方式？
问题就在于，决定了协议族并不能同时决定数据传输方式，换言之，socket函数第一个参数PF_INET协议族中也存在多种数据传输方式。

下面介绍2种具有代表性的数据传输方式。这是理解好套接字的重要前提，请各位务必掌握。

**套接字类型1：面向连接的套接字（SOCK_STREAM）**

如果向socket函数的第二个参数传递SOCK_STREAM，将创建面向连接的套接字。面向连接的套接字到底具有哪些特点呢？

- 传输过程中数据不会消失。
- 按序传输数据。
- 传输的数据不存在数据边界（Boundary）。

传输数据的计算机可以通过3次调用write函数传递了100字节的数据，但接收数据的计算机仅通过1次read函数调用就接收了全部100个字节。

收发数据的套接字内部有缓冲（buffer），简言之就是字节数组。通过套接字传输的数据将保存到该数组。
因此，收到数据并不意味着马上调用read函数。只要不超过数组容量，则有可能在数据填充满缓冲后通过1次read函数调用读取全部，也有可能分成多次read函数调用进行读取。
也就是说，在面向连接的套接字中，read函数和write函数的调用次数并无太大意义。所以说面向连接的套接字不存在数据边界。稍后将给出示例以查看该特性。

套接字缓冲已满是否意味着数据丢失？

> 之前讲过，为了接收数据，套接字内部有一个由字节数组构成的缓冲。如果这个缓冲被接收的数据填满会发生什么事情？之后传递的数据是否会丢失？
>
> 首先调用read函数从缓冲读取部分数据，因此，缓冲并不总是满的。但如果read函数读取速度比接收数据的速度慢，则缓冲有可能被填满。
> 此时套接字无法再接收数据，但即使这样也不会发生数据丢失，因为传输端套接字将停止传输。
> 也就是说，面向连接的套接字会根据接收端的状态传输数据，如果传输出错还会提供重传服务。因此，面向连接的套接字除特殊情况外不会发生数据丢失。

面向连接的套接字还有如下特点：“套接字连接必须一一对应。”

面向连接的套接字只能与另外一个同样特性的套接字连接。用一句话概括面向连接的套接字如下："可靠的、按序传递的、基于字节的面向连接的数据传输方式的套接字"

**套接字类型2：面向消息的套接字（SOCK_DGRAM）**

如果向socket函数的第二个参数传递SOCK_DGRAM，则将创建面向消息的套接字。面向消息的套接字可以比喻成高速移动的摩托车快递。

- 强调快速传输而非传输顺序。
- 传输的数据可能丢失也可能损毁。
- 传输的数据有数据边界。□限制每次传输的数据大小。

众所周知，快递行业的速度就是生命。用摩托车发往同一目的地的2件包裹无需保证顺序，只要以最快速度交给客户即可。
这种方式存在损坏或丢失的风险，而且包裹大小有一定限制。因此，若要传递大量包裹，则需分批发送。
另外，如果用2辆摩托车分别发送2件包裹，则接收者也需要分2次接收。这种特性就是“传输的数据具有数据边界”。

以上就是面向消息的套接字具有的特性。即，面向消息的套接字比面向连接的套接字具有更快的传输速度，但无法避免数据丢失或损毁。

另外，每次传输的数据大小具有一定限制，并存在数据边界。存在数据边界意味着接收数据的次数应和传输次数相同。

面向消息的套接字特性总结如下：
"不可靠的、不按序传递的、以数据的高速传输为目的的套接字"。

另外，面向消息的套接字不存在连接的概念，这一点将在以后章节介绍。

**协议的最终选择**

下面讲解socket函数的第三个参数，该参数决定最终采用的协议。各位是否觉得有些困惑？

前面已经通过socket函数的前两个参数传递了协议族信息和套接字数据传输方式，这些信息还不足以决定采用的协议吗？为什么还需要传递第3个参数呢？

正如各位所想，传递前两个参数即可创建所需套接字。所以大部分情况下可以向第三个参数传递0，除非遇到以下这种情况：
“同一协议族中存在多个数据传输方式相同的协议”

数据传输方式相同，但协议不同。此时需要通过第三个参数具体指定协议信息。
下面以前面讲解的内容为基础，构建向socket函数传递的参数。首先创建满足如下要求的套接字：
"IPv4协议族中面向连接的套接字"

IPv4与网络地址系统相关，关于这一点将给出单独说明，目前只需记住：本书是基于IPv4展开的。
参数PF_INET指IPv4网络协议族，SOCK_STREAM是面向连接的数据传输。满足这2个条件的协议只有IPPROTO_TCP，因此可以如下调用socket函数创建套接字，这种套接字称为TCP套接字。

```c
int tcp_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

```

下面创建满足如下要求的套接字：
“IPv4协议族中面向消息的套接字”

SOCK_DGRAM指的是面向消息的数据传输方式，满足上述条件的协议只有IPPROTO_UDP。因此，可以如下调用socket函数创建套接字，这种套接字称为UDP套接字。

```c
int udp_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

```

前面进行了大量描述以解释这两行代码，这是为了让大家理解它们创建的套接字的特性。

**面向连接的套接字：TCP套接字示例**

其他章节将讲解UDP套接字，此处只给出面向连接的TCP套接字示例。本示例是在第1章的如下2个源文件基础上修改而成的。

- hello_server.c → tcp_server.c：无变化！
- hello_client.c → tcp_client.c：更改read函数调用方式！

之前的hello_server.c和hello_client.c是基于TCP套接字的示例，现调整其中一部分代码，以验证TCP套接字的如下特性：
“传输的数据不存在数据边界。”

为验证这一点，需要让write函数的调用次数不同于read函数的调用次数。因此，在客户端中分多次调用read函数以接收服务器端发送的全部数据。

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
    int strLen = 0;
    int readLen = 1;
    int idx = 0;
    // 修改读取方式，每次只读取1个字节
    while (readLen) {
        readLen = read(clntSock, &msg[idx++], 1);
        if (readLen == -1) {
            std::cout << "read 错误！" << std::endl;
            return 0;
        }
        strLen += readLen;
    }

    std::cout << "接收数据：" << msg << std::endl;

    close(clntSock);

    return 0;
}


```


## Windows平台下的实现及验证

前面讲过的套接字类型及传输特性与操作系统无关。Windows平台下的实现方式也类似，不需要过多说明，只需稍加了解socket函数返回类型即可。

**Windows 操作系统的socket函数**

Windows的函数名和参数名都与Linux平台相同，只是返回值类型稍有不同。再次给出socket 函数的声明。

```c
#include <winsock2.h>
SOCKET socket(int af, int type, int protocol);
// 成功时返回socket 句柄，失败时返回INVALID_SOCKET。

```

该函数的参数种类及含义与Linux的socket函数完全相同，故省略，只讨论返回值类型。
可以看出返回值类型为SOCKET，此结构体用来保存整数型套接字句柄值。
实际上，socket函数返回整数型数据，因此可以通过int型变量接收，就像在Linux中做的一样。
但考虑到以后的扩展性，定义为SOCKET数据类型，希望各位也使用SOCKET结构体变量保存套接字句柄，这也是微软希望看到的。
以后即可将SOCKET视作保存套接字句柄的一个数据类型。

同样，发生错误时返回INVALID_SOCKET，只需将其理解为提示错误的常数即可。其实际值为-1，但值是否为-1并不重要，除非编写如下代码。

```c
SOCKET soc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
if(soc == -1)
    ErrorHandling("...");

```

如果这样编写代码，那么微软定义的INVALID_SOCKET常数将失去意义！应该如下编写，这样，即使日后微软更改INVALID_SOCKET常数值，也不会发生问题。

```c
SOCKET soc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
if(soc == INVALID_SOCKET)
    ErrorHandling("...");

```

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
    int strLen = 0;
    int readLen = 1;
    int idx = 0;
    while (readLen) {
        readLen = recv(clntSock, &msg[idx++], 1, 0);
        if (readLen == -1) {
            closesocket(clntSock);
            std::cout << "recv 错误" << std::endl;
            return 0;
        }
        strLen += readLen;
    }

    std::cout << "接收：" << msg << std::endl;
    closesocket(clntSock);

    WSACleanup();

    return 0;
}

```



