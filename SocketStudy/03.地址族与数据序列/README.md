# 地址族与数据序列

## 分配给套接字的IP地址与端口号

IP是Internet Protocol(网络协议)的简写，是为收发网络数据而分配给计算机的值。
端口号并非赋予计算机的值，而是为区分程序中创建的套接字而分配给套接字的序号。下面逐一讲解。

**网络地址(Internet Address)**

为使计算机连接到网络并收发数据，必需向其分配IP地址。IP地址分为两类

- IPv4 (Internet Protocol version 4) 4字节地址族
- IPv6 (Internet Protocol version 6) 16字节地址族

IPv4与IPv6的差别主要是表示IP地址所用的字节数，目前通用的地址族为IPv4。
IPv6是为了应对2010年前后IP地址耗尽的问题而提出的标准，即便如此，现在还是主要使用IPv4，IPV6的普及将需要更长时间。

IPv4标准的4字节IP地址分为网络地址和主机(指计算机)地址，且分为A、B、C、D、E等类型。

网络地址(网络ID)是为区分网络而设置的一部分IP地址。
假设向WWW.SEMI.COM公司传输数据，该公司内部构建了局域网，把所有计算机连接起来。
因此，首先应向SEMI.COM网络传输数据，也就是说，并非一开始就浏览所有4字节IP地址，进而找到目标主机。
而是仅浏览4字节IP地址的网络地址，先把数据传到SEMICOM的网络。
SEMLCOM网络(构成网络的路由器)按收到数据后，浏览传输数据的主机地址(主机ID并将数据传给目标计算机。

某主机向203.211.172.103和203211.217.202传输数据其中203.211.172和203.211.217为该网络的网络地址(稍后将给出网络地址的区分方法)。
所以，“向相应网络传输数据”实际上是向构成网络的路由器(Router)或交换机(Switch)传递数据，由接收数据的路由器根据数据中的主机地址向目标主机传递数据。

**网络地址分类与主机地址边界**

只需通过IP地址的第一个字节即可判断网络地址占用的字节数，因为我们根据IP地址的边界区分网络地址，如下所示。

- A类地址的首字节范围:0~127
- B类地址的首字节范围:128~191
- C类地址的首字节范围:192~223

还有如下这种表述方式。

- A类地址的首位以0开始
- B类地址的前2位以10开始
- C类地址的前3位以110开始

正因如此，通过套接字收发数据时，数据传到网络后即可轻松找到正确的主机。

**用于区分套接字的端口号**

IP用于区分计算机，只要有IP地址就能向目标主机传输数据，但仅凭这些无法传输给最终的应用程序。
假设各位欣赏视频的同时在网上冲浪，这时至少需要1个接收视频数据的套接字和1个接收网页信息的套接字。
问题在于如何区分二者。简言之，传输到计算机的网络数据是发给播放器，还是发送给浏览器?让我们更准确地描述问题。
假设各位开发了如下应用程序:

“我开发了收发数据的P2P程序，该程序用块单位分割1个文件，从多台计算机接收数据。”

假设各位对P2P有一定了解，即便不清楚也无所谓。如上所述，若想接收多台计算机发来的数据，则需要相应个数的套接字。那如何区分这些套接字呢?

计算机中一般配有NIC(NetworkInterfaceCard，网络接口卡)数据传输设备。
通过NIC向计算机内部传输数据时会用到IP。操作系统负责把传递到内部的数据适当分配给套接字，这时就要利用端口号。也就是说，通过NIC接收的数据内有端口号，操作系统正是参考此端口号把数据传输给相应端口的套接字。

端口号就是在同一操作系统内为区分不同套接字而设置的，因此无法将1个端口号分配给不同套接字。
另外，端口号由16位构成，可分配的端口号范围是0-65535。但0-1023是知名端口(Well-known PORT)，一般分配给特定应用程序，所以应当分配此范围之外的值。
另外，虽然端口号不能重复，但TCP套接字和UDP套接字不会共用端口号，所以允许重复。例如:如果某TCP套接字使用9190号端口，则其他TCP套接字就无法使用该端口号，但UDP套接字可以使用。

总之，数据传输目标地址同时包含IP地址和端口号，只有这样，数据才会被传输到最终的目的应用程序(应用程序套接字)。



## 地址信息的表示

应用程序中使用的IP地址和端口号以结构体的形式给出了定义。
本节将以IPV4为中心，围绕此结构体讨论目标地址的表示方法。

**表示IPv4地址的结构体**

填写地址信息时应以如下提问为线索进行，各位读过下列对话后也会同意这一点。

- 问题1:“采用哪一种地址族?
- 答案1:“基于IPv4的地址族。

- 问题2:“IP地址是多少?”
- 答案2:“211.204.214.76。”

- 问题3:“端口号是多少?”
- 答案3:“2048。

结构体定义为如下形态就能回答上述提问，此结构体将作为地址信息传递给bind函数。

```c
typedef unsigned short int  sa_family_t;// 16位
typedef uint32_t            in_addr_t;  // 32位

struct in_addr
{
    in_addr_t           s_addr;         // 32位IPv4地址
};

struct sockaddr_in
{
    sa_family_t         sin_family;     // 地址族（Address Family）
    uint16_t            sin_port;       // 16位TCP/UDP端口号
    struct in_addr      sin_addr;       // 32位IP地址
    char                sin_zero[8];    // 不使用
};

```

讲解以上2个结构体前先观察一些数据类型。uint16_t、in_addr_t等类型可以参考POSIX(Portable Operating System Interface，可移植操作系统接口)。POSIX是为UNIX系列操作系统设立的标准，它定义了一些其他数据类型

从这些数据类型声明也可掌握之前结构体的含义。那为什么需要额外定义这些数据类型呢?
如前所述，这是考虑到扩展性的结果。如果使用int32类型的数据，就能保证在任何时候都占用4字节，即使将来用64位表示int类型也是如此。

结构体sockaddr_in的成员分析

- 成员sin_family

  每种协议族适用的地址族均不同。比如，IPv4使用4字节地址族，IP6使用16字节地址族。可以参考表保存sin_family地址信息。

  | 地址族(Address Family)  | 含义                                  |
  | -------                 | -------                               |
  | AF_INET                 | IPv4网络协议中使用的地址族            |
  | AF_INET6                | IPv6网络协议中使用的地址族            |
  | AF_LOCAL                | 本地通信中采用的UNIX协议的地址族      |

  AFLOCAL只是为了说明具有多种地址族而添加的，希望各位不要感到太突然

- 成员sin_port

  该成员保存16位端口号,重点在于,它以网络字节序保存(关于这一点稍后将给出详细说明)。

- 成员sin_addr

  该成员保存32位IP地址信息，且也以网络字节序保存。为理解好该成员，应同时观察结构体inaddr。但结构体inaddr声明为uint32t，因此只需当作32位整数型即可。

- 成员sin_zero

  无特殊含义。只是为使结构体sockaddr_in的大小与sockaddr结构体保持一致而插入的成员必需填充为0，否则无法得到想要的结果。后面会另外讲解sockaddr。

从之前介绍的代码也可看出，sockaddr in结构体变量地址值将以如下方式传递给bind函数。
稍后将给出关于bind函数的详细说明，希望各位重点关注参数传递和类型转换部分的代码。

```c
struct sockaddrin serv _addr;
// ...
if(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(servaddr)) == -1)
    error_handling("bind() error");
// ...

```

此处重要的是第二个参数的传递。实际上，bind函数的第二个参数期望得到sockaddr结构体变量地址值，包括地址族、端口号、IP地址等。从下列代码也可看出，直接向sockaddr结构体填充这些信息会带来麻烦。

```c
struct sockaddr
{
    sa_family_t         sin_family;     // 地址族（Address Family）
    char                sa_data[14];    // 地址信息
};


```


此结构体成员sa_data保存的地址信息中需包含IP地址和端口号，剩余部分应填充0这也是bind函数要求的。
而这对于包含地址信息来讲非常麻烦，继而就有了新的结构体sockaddr_in。
若按照之前的讲解填写sockaddr_in结构体，则将生成符合bind函数要求的字节流。
最后转换为sockaddr型的结构体变量，再传递给bind函数即可。

> sockaddr_in是保存IPv4地址信息的结构体。那为何还需要通过sin_family单独指定地址族信息呢?
> 这与之前讲过的sockaddr结构体有关。结构体sockaddr并非只为IPv4设计这从保存地址信息的数组sa_data长度为14字节也可看出。
> 因此，结构体sockaddr要求在sin_family中指定地址族信息。为了与sockaddr保持一致，sockaddr_in结构体中也有地址族信息。






## 网络字节序与地址变换

不同CPU中，4字节整数型值1在内存空间的保存方式是不同的。4字节整数型值1可用2进制表示如下。

```txt
00000000 00000000 00000000 00000001
```

有些CPU以这种顺序保存到内存，另外一些CPU则以倒序保存

```txt
00000001 00000000 00000000 00000000
```

若不考虑这些就收发数据则会发生问题，因为保存顺序的不同意味着对接收数据的解析顺序也不同。

**字节序(Order)与网络字节序**

CPU向内存保存数据的方式有2种，这意味着CPU解析数据的方式也分为2种。

- 大端序(BigEndian):高位字节存放到低位地址
- 小端序(LittleEndian):高位字节存放到高位地址。

在通过网络传输数据时约定统一方式，这种约定称为网络字节序(Network Byte Order)，非常简单一统一为大端序。

即，先把数据数组转化成大端序格式再进行网络传输。因此，所有计算机接收数据时应识别该数据是网络字节序格式，小端序系统传输数据时应转化为大端序排列方式

**字节序转换 (Endian Conversions)**

相信大家已经理解了为何要在填充sockadr_in结构体前将数据转换成网络字节序。接下来介绍帮助转换字节序的函数。

- unsigned short htons(unsigned short);
- unsigned short ntohs(unsigned short);
- unsigned long htonl(unsigned long);
- unsigned long ntohl(unsigned long);

通过函数名应该能掌握其功能，只需了解以下细节

- htons中的h代表主机(host)字节序。
- htons中的n代表网络(network)字节序。

另外，s指的是short，l指的是long(Linux中long类型占用4个字节，这很关键)。
因此，htons是h、to、n、s的组合，也可以解释为“把short型数据从主机字节序转化为网络字节序”。

再举个例子，ntohs可以解释为“把short型数据从网络字节序转化为主机字节序”

通常，以s作为后缀的函数中，s代表2个字节short，因此用于端口号转换:以1作为后缀的函数中，1代表4个字节，因此用于IP地址转换。另外，有些读者可能有如下疑问:

“我的系统是大端序的，为sockaddr_in结构体变量赋值前就不需要转换字节序了吧”

这么说也不能算错。但我认为，有必要编写与大端序无关的统一代码。
这样，即使在大端序系统中，最好也经过主机字节序转换为网络字节序的过程。
当然，此时主机字节序与网络字节序相同，不会有任何变化。下面通过示例说明以上函数的调用过程。

```c++
#include <arpa/inet.h>
#include <iostream>

int main(int argc, char* argv[])
{
    unsigned short hostPort = 0x1234;
    unsigned short netPort = htons(hostPort);
    unsigned long hostAddr = 0x12345678;
    unsigned long netAddr = htonl(hostAddr);

    std::cout << "Host ordered port: " << hostPort << std::endl;
    std::cout << "Network ordered port: " << netPort << std::endl;
    std::cout << "Host ordered address: " << hostAddr << std::endl;
    std::cout << "Network ordered address: " << netAddr << std::endl;

    return 0;
}


```

数据在传输之前都要经过转换吗

> 也许有读者认为：“既然数据传输采用网络字节序，那在传输前应直接把数据转换成网络字节序，接收的数据也需要转换成主机字节序再保存。
> "如果数据收发过程中没有自动转换机制，那当然需要程序员手动转换。这光想想就让人觉得可怕，难道真要强求程序员做这些事情吗？
> 实际上没必要，这个过程是自动的。除了向sockaddr_in结构体变量填充数据外，其他情况无需考虑字节序问题。





## 网络地址的初始化与分配

前面已讨论过网络字节序，接下来介绍以bind函数为代表的结构体的应用。

**将字符串信息转换为网络字节序的整数型**

sockaddr_in中保存地址信息的成员为32位整数型。因此，为了分配IP地址，需要将其表示为32位整数型数据。
这对于只熟悉字符串信息的我们来说实非易事。各位可以尝试将IP地址201.211.214.36转换为4字节整数型数据。

对于IP地址的表示，我们熟悉的是点分十进制表示法（Dotted Decimal Notation），而非整数型数据表示法。
幸运的是，有个函数会帮我们将字符串形式的IP地址转换成32位整数型数据。此函数在转换类型的同时进行网络字节序转换。

```c
#include <arpa/inet.h>
in_addr_t inet_addr(const char * string);
// 成功时返回32位大端序整数型值，失败时返回INADDR_NONE。

```

如果向该函数传递类似"211.214.107.99"的点分十进制格式的字符串，它会将其转换为32位整数型数据并返回。
当然，该整数型值满足网络字节序。另外，该函数的返回值类型in_addr_t在内部声明为32位整数型。下列示例表示该函数的调用过程。

```c++
#include <arpa/inet.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    std::string addr1 = "1.2.3.4";
    unsigned long int convAddr = inet_addr(addr1.c_str());
    if (convAddr == INADDR_NONE) {
        std::cout << "Address1 error!" << std::endl;
    } else {
        std::cout << "网络字节序的整数地址1：" << convAddr << std::endl;
    }

    std::string addr2 = "1.2.3.256";
    convAddr = inet_addr(addr2.c_str());
    if (convAddr == INADDR_NONE) {
        std::cout << "Address2 error!" << std::endl;
    } else {
        std::cout << "网络字节序的整数地址2：" << convAddr << std::endl;
    }

    return 0;
}


```

从运行结果可以看出，inet_addr函数不仅可以把IP地址转成32位整数型，而且可以检测无效的IP地址。另外，从输出结果可以验证确实转换为网络字节序。

inet_aton函数与inet_addr函数在功能上完全相同，也将字符串形式IP地址转换为32位网络字节序整数并返回。只不过该函数利用了in_addr结构体，且其使用频率更高。

```c++
#include <arpa/inet.h>
int  inet_aton(const  char * string, struct in_addr * addr);
// 成功时返回1（true），失败时返回0（false）
// string   含有需转换的IP地址信息的字符串地址值。
// addr     将保存转换结果的in_addr结构体变量的地址值。

```

实际编程中若要调用inet_addr函数，需将转换后的IP地址信息代入sockaddr_in结构体中声明的in_addr结构体变量。
而inet_aton函数则不需此过程。原因在于，若传递in_addr结构体变量地址值，函数会自动把结果填入该结构体变量。
通过示例了解inet_aton函数调用过程。



## 基于Windows的实现





