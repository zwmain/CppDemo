# 01.Socket基础

基本socket接口使用

## socket()函数

创建一个socket套接字，错误会返回-1，成功会返回套接字的整数值。

函数原型

```c
#include <sys/socket.h>
int socket(int af, int type, int protocol);
```
1. `af`为地址族（Address Family），即IP地址类型，常用取值为`AF_INET`和`AF_INET6`。前者代表IPv4地址后者则是IPv6地址。
2. `type`为数据传输类型，常用取值为`SOCK_STREAM`（面向数据流的传输）和`SOCK_DGRAM`（面向数据报的传输）。
3. `protocol`为传输使用的协议，常用的有`IPPROTO_TCP`和`IPPTOTO_UDP`，分别为TCP协议和UDP协议

其实，前两个参数就可以确定第三个参数了，面向数据流的只有TCP协议，面向数据报的只有UDP协议，所以在实际使用中，最后一个参数一般填0。

## bind()函数

```c
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

```

绑定一个套接字到某一个IP地址和端口上

## listen()函数

```c
#include <sys/socket.h>
int listen(int sockfd, int backlog);

```

监听一个套接字

## accept()函数

```c
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)

```

阻塞，直到接收到一个请求

## connect()函数

```c
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

```

## recv()函数

从套接字读取数据

## close()函数

关闭套接字

## sockaddr结构体

```c

struct sockaddr {
    sa_family_t sa_family;    // 地址族
    char        sa_data[14];  // 端口2字节+IP地址4字节
}

```

## sockaddr_in结构体

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

struct sockaddr_in {
    sa_family_t    sin_family; /* 地址族: AF_INET */
    in_port_t      sin_port;   /* 网络字节序的端口 */
    struct in_addr sin_addr;   /* IP地址 */
};

/* Internet address */
struct in_addr {
    uint32_t       s_addr;     /* 网络字节序的地址 */
};
```

## 地址转换

```c++
#include <arpe/inet.h>
// 将点分十进制的ip地址转化为用于网络传输的数值格式
// 返回值：若成功则为1，若输入不是有效的表达式则为0，若出错则为-1
int inet_pton(int family, const char *strptr, void *addrptr);


// 将数值格式转化为点分十进制的ip地址格式
// 返回值：若成功则为指向结构的指针，若出错则为NULL
const char * inet_ntop(int family, const void *addrptr, char *strptr, size_t len);

```
