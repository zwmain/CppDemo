# 14.多播与广播

## 14.1 多播

多播（Multicast）方式的数据传输是基于UDP完成的。
因此，与UDP服务端/客户端的实现方式非常接进。
区别在于，UDP数据传输以单一目标进行，而多播数据同时传递到加入特定组的大量主机。
也就是说，多播同时向多个主机传递数据。

### 多播的数据传输方式及流量方面的优点

- 多播服务器端针对特定多播组，只发送1次数据。
- 只需发送1次数据，该组内的所有客户端都会接收数据。
- 多播组数可以在IP地址范围内任意增加。
- 加入特定组即可接收发往该多播组的数据。

多播组是D类IP地址（224.0.0.0~239.255.255.255），“加入多播组”可以理解为：
“在D类IP地址中，我希望接收发往目标239.234.218.234的多播数据”

多播是基于UDP完成的，数据包格式与UDP数据包相同。

不同的是，多播数据会被路由器复制并传送到多个主机。

### 路由和TTL（Time to Live，生存时间）以及加入组的方法

TTL并不是时间，而是最多路由的次数，也就是最多可以经过多少个路由器。

设置TTL是通过套接字可选项完成的。

与设置TTL相关的协议层为`IP_PROTO_IP`，选项名为`IP_MULTICAST_TTL`。

因此，可以用如下代码设置TTL，这里设置为64。

```c
int time_live = 64;
int send_sock = socket(PF_INET, SOCK_DGRAM, 0);
setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));

```

加入多播组也通过设置套接字完成。加入多播组相关的协议层为`IPPROTO_IP`，选项名为`IP_ADD_MEMBERSHIP`。

代码示例如下：

```c
int recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
struct ip_mreq join_adr;
join_adr.imr_multiaddr.s_addr = "多播组地址信息";
join_adr.imr_interface.s_addr = "加入多播组的主机地址信息";
setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));

```

`ip_mrep`结构体详解：

```c
struct ip_mreq
{
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
};

```

第一个成员`imr_multiaddr`中写入加入的组IP地址。
第二个成员`imr_interface`是加入该组的套接字所属主机的IP地址，可以使用`INADDR_ANY`。

### 实现多播Sender和Receiver

多播中用“发送者”和“接收者”替代服务端和客户端。

- Sender：向AAA组广播（Broadcasting）文件中保存的新闻信息。
- Receiver：接收传递到AAA组的新闻信息。

Sender比Receiver简单，因为Receiver需要经过加入组的过程，而Sender只需要创建UDP套接字，并向多播地址发送数据。

```c++
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

constexpr int TTL = 64;
constexpr int BUF_SIZE = 30;

#ifndef FILEPATH
#define FILEPATH "../news.txt"
#endif

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << ' ';
    }
    std::cout << std::endl;
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <GroupIP> <PORT>" << std::endl;
        return 0;
    }

    int send_sock = socket(PF_INET, SOCK_DGRAM, 0);

    // 初始化目标地址，和普通的目标地址没有区别，只是传进来的参数是一个多播地址而已
    sockaddr_in mul_adr;
    std::memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(std::atoi(argv[2]));

    // 设置套接字TTL
    int time_live = TTL;
    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));

    std::ifstream fi { FILEPATH };
    while (!fi.eof()) {
        std::string msg;
        fi >> msg;
        sendto(send_sock, msg.c_str(), msg.size(), 0, (sockaddr*)&mul_adr, sizeof(mul_adr));
        std::cout << msg << std::endl;
        sleep(2);
    }
    close(send_sock);
    return 0;
}

```

## 14.2 广播

待完成
