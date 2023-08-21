# 13.多种IO函数

之前，基于Linux的使用read&write函数完成数据待I/O,
基于Windows的则使用send&recv函数。
Linux也将使用send&recv函数，并讲解与read&write函数相比的有点所在。
还有其他I/O函数

## 13.1 send & recv 函数

### Linux中的send & recv

Linux下的`send & recv`函数与Windows下的同名函数并无区别。

```c
#include <sys/socket.h>
ssize_t send(int sockfd, const void* buf, size_t nbytes, int flags);
// 成功时返回发送的字节数，失败时返回-1
// sockfd           表示与数据传输对象的连接的套接字文件描述符
// buf              保存待传输数据待缓冲地址值
// nbytes           待传输的字节数
// flags            传输数据时指定的可选项信息
```

该函数与Windows下的，在参数顺序、含义、使用方法都完全相同；`recv`函数也一样。

```c
#include <sys/socket.h>
ssize_t recv(int sockfd, void* buf, size_t nbytes, int flags);
// 成功时返回发送的字节数，失败时返回-1
// sockfd           表示与数据传输对象的连接的套接字文件描述符
// buf              保存接收数据的缓冲地址值
// nbytes           可接收的字节数
// flags            接收数据时指定的可选项信息
```

`send`函数和`recv`函数的最后一个参数是收发数据时的可选项。该可选项可利用**位或运算符**同时传递多个信息。

| 可选项        | 含义                                               | send   | recv   |
| ------------- | -------------------------------------------------- | ------ | ------ |
| MSG_OOB       | 用于传输带外数据                                   | 支持   | 支持   |
| MSG_PEEK      | 验证输入缓冲中是否存在接收的数据                   | 不支持 | 支持   |
| MSG_DONTROUTE | 数据传输过程中不参照路由表，在本地网络中寻找目的地 | 支持   | 不支持 |
| MSG_DONTWAIT  | 调用I/O函数时不阻塞，用于使用非阻塞I/O             | 支持   | 支持   |
| MSG_WAITALL   | 防止函数返回，直到接收全部请求的字节数             | 不支持 | 支持   |

注意，不同操作系统对上述选项的支持并不一样。

### MSG_OOB: 发送紧急消息

MSG_OOB可选项用于发送“带外数据”紧急消息。

就像医院里的急诊一样，须要紧急处理时，应采用不同的处理方法和通道。

MSG_OOB可选项就用于创建特殊发送方法和通道以发送紧急消息。

下列示例将通过MSG_OOB可选项收发数据。使用MSG_OOB时需要一些拓展知识，这部分内容通过源代码进行讲解。

```c
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[])

{
    int sock;
    struct sockaddr_in recv_adr;
    if (argc != 3) {

        printf("Usage:%s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
        error_handling("connect()error!");

    write(sock, "123", strlen("123"));
    send(sock, "4", strlen("4"), MSG_OOB);
    write(sock, "567", strlen("567"));
    send(sock, "890", strlen("890"), MSG_OOB);
    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}


```

> 第29~32行：传输数据。第30和第32行紧急传输数据。正常顺序应该是123、4、567、890，但紧急传输了4和890，由此可知接收顺序也将改变。

紧急消息的发送比接收要简单，只需要在调用send函数时指定MSG_OOB可选项。接收紧急消息的过程要相对复杂一些。

```c
#define _XOPEN_SOURCE
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUF_SIZE 30
void error_handling(char* message);
void urg_handler(int signo);

int acpt_sock;
int recv_sock;

int main(int argc, char* argv[])
{
    struct sockaddr_in recv_adr, serv_adr;
    int str_len, state;
    socklen_t serv_adr_sz;
    struct sigaction act;
    char buf[BUF_SIZE];
    if (argc = 2) {
        printf("usage : %s <port>\n", argv[0]);
        exit(1);
    }

    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(acpt_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
        error_handling("bind() error");
    listen(acpt_sock, 5);

    serv_adr_sz = sizeof(serv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&serv_adr, &serv_adr_sz);

    fcntl(recv_sock, F_SETOWN, getpid());
    state = sigaction(SIGURG, &act, 0);

    while (str_len = recv(recv_sock, buf, sizeof(buf), 0) != 0)
    {
        if (str_len == -1)
            continue;
        buf[str_len] = 0;
        puts(buf);
    }
    close(recv_sock);
    close(acpt_sock);
    return;
}

void urg_handler(int signo)
{
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
    buf[str_len] = 0;
    printf("Urgent message:%s \n", buf);
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

```

> 第29、47行：该示例中，需要重点观察SIGURG信号相关部分。收到MSG_OOB紧急消息时，操作系统将产生SIGUTG信号，
> 并调用注册的信号处理函数。另外需要注意的是，第61行的信号处理函数内部调用了接收紧急消息的recv函数。
>
> 第46行：调用fcntl函数，关于此函数将单独说明。

```c
fcntc(recv_sock, F_SETOWN, getpid());
```
`fcntl`函数用于控制文件描述符，但上述调用语句的含义如下：

将文件描述符`recv_sock`指向的套接字拥有者`F_SETOWN`，改为把`getpid`函数返回值所指向的进程。

### 检查输入缓冲

同时设置`MSG_PEEK`选项和`MSG_DONTWAIT`选项，以验证输入缓冲是否存在接收的数据。
设置`MSG_PEEK`选项并调用`recv`函数时，即使读取了输入缓冲的数据也不会删除。因此，该选项通常与`MSG_DONTWAIT`合作，用于调用以非阻塞方式验证待读数据存在与否的函数。

示例：

`peek_send.cpp`

```c++
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <IP> <PORT>" << std::endl;
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in send_adr;
    std::memset(&send_adr, 0, sizeof(send_adr));
    send_adr.sin_family = AF_INET;
    send_adr.sin_addr.s_addr = inet_addr(argv[1]);
    send_adr.sin_port = htons(std::atoi(argv[2]));

    int rtn = connect(sock, (sockaddr*)&send_adr, sizeof(send_adr));
    if (rtn == -1) {
        std::cout << "connect error" << std::endl;
        return 0;
    }

    std::string msg = "123";
    write(sock, msg.c_str(), msg.size());
    close(sock);

    return 0;
}

```

`peek_recv.cpp`

```c++
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 30;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <PORT>" << std::endl;
        return 0;
    }

    int acpt_sock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in acpt_adr;
    std::memset(&acpt_adr, 0, sizeof(acpt_adr));
    acpt_adr.sin_family = AF_INET;
    acpt_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_adr.sin_port = htons(std::atoi(argv[1]));

    int rtn = bind(acpt_sock, (sockaddr*)&acpt_adr, sizeof(acpt_adr));
    if (rtn == -1) {
        std::cout << "bind error" << std::endl;
        return 0;
    }

    listen(acpt_sock, 5);

    sockaddr_in recv_adr;
    std::memset(&recv_adr, 0, sizeof(recv_adr));
    socklen_t recv_sz = sizeof(recv_adr);

    int recv_sock = accept(acpt_sock, (sockaddr*)&recv_adr, &recv_sz);

    char buf[BUF_SIZE] = { 0 };
    int str_sz = 0;
    while (true) {
        str_sz = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK | MSG_DONTWAIT);
        if (str_sz > 0) {
            break;
        }
    }

    buf[str_sz] = 0;
    std::cout << "Buffering " << str_sz << " bytes: " << buf << std::endl;

    str_sz = recv(recv_sock, buf, sizeof(buf) - 1, 0);
    buf[str_sz] = 0;
    std::cout << "Read again: " << buf << std::endl;

    close(acpt_sock);
    close(recv_sock);

    return 0;
}

```

运行结果可以验证，仅发送1次的数据被读取了2次，因为第一次调用了`recv`函数时设置了`MSG_PEEK`可选项。


## 13.2 readv & writev 函数

`readv`和`writev`函数的功能可概括为：对数据进行整合传输以及发送

通过`writev`可以将分散保存在多个缓冲中的数据一并发送，通过`readv`函数可以由多个缓冲区分别接收。
因此，可以适当使用这2个函数，减少IO调用次数。

```c
#include <sys/uio.h>

ssize_t writev(int fileds, const struct iovec* iov, int iovcnt);

// 成功时返回发送的字节数，失败时返回-1
// filedes      表示数据传输对象的套接字文件描述符
// iov          iovec结构体数组的地址值，结构体iovec中包含待发送数据的位置和大小信息
// iovcnt       向第二个参数传递的数组长度

// iovec结构体
struct iovec
{
    void* iov_base; // 缓冲地址
    size_t iov_len; // 缓冲大小
}

```

iovec由保存待发送数据的缓冲地址值和数据长度信息构成。

```c
#include <iostream>
#include <sys/uio.h>

int main(int argc, char* argv[])
{
    iovec vec[2];
    char buf1[] = "ABCDEFG";
    char buf2[] = "1234567";
    vec[0].iov_base = buf1;
    vec[0].iov_len = 3;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 4;
    int strLen = writev(1, vec, 2); // 文件描述符为1表示向控制台输出数据
    std::cout << std::endl;
    std::cout << "Write bytes: " << strLen << std::endl;
    return 0;
}

```

## 13.3 基于Windows的实现

待完成
