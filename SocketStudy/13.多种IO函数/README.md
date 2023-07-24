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


## 13.2 readv & writev 函数

待完成

## 13.3 基于Windows的实现

待完成
