# 域名及网络地址

## 域名系统

DNS是对IP地址和域名进行相互转换的系统，其核心是DNS服务器。

什么是域名

提供网络服务的服务器端也是通过IP地址区分的，但几乎不可能以非常难记的IP地址形式交换服务器端地址信息。因此，将容易记、易表述的域名分配并取代IP地址。

DNS服务器

在浏览器地址栏中输人Naver网站的IP地址222.122.195.5即可浏览Naver网站主页。但我们通常输人Naver网站的域名wwwnavercom访问网站。二者之间究竟有何区别?

从进入Naver网站主页这一结果上看，没有区别，但接入过程不同。域名是赋予服务器端的虚拟地址，而非实际地址。因此，需要将虚拟地址转化为实际地址。那如何将域名变为IP地址呢?DNS服务器担此重任，可以向DNS服务器请求转换地址。

“请问DNS服务器，wwwnavercom的IP地址是多少?”

所有计算机中都记录着默认DNS服务器地址，就是通过这个默认DNS服务器得到相应域名的IP地址信息。在浏览器地址栏中输入域名后，浏览器通过默认DNS服务器获取该域名对应的IP地址信息，之后才真正接入该网站。

> ping & nslookup
>
> 除非商业需要，否则一般不会轻易改变服务器域名，但会相对频繁地改变服务器IP地址。如果各位想了解某个域名对应的IP地址信息，可以在控制台窗口输入如下内容
>
> ping wwwnaver.com
>
> 这样即可知道某一域名对应的IP地址。ping命令用来验证IP数据报是否到达目的地，但执行过程中会同时经过“域名到IP地址”的转换过程，因此可以通过此命令查看1P地址。另外，若各位想知道自己计算机中注册的默认DNS服务器地址，可以输入如下命令
>
> nslookup
>
> 在Linux系统中输入上述命令后，会提示进一步输入信息，此时可以输入server得到默认DNS服务器地址。

计算机内置的默认DNS服务器并不知道网络上所有域名的IP地址信息。若该DNS服务器无法解析，则会询问其他DNS服务器，并提供给用户。

默认DNS服务器收到自已无法解析的请求时，向上级DNS服务器询问。通过这种方式逐级向上传递信息，到达顶级DNS服务器一一根DNS服务器时，它知道该向哪个DNS服务器询问。向下级DNS传递解析请求，得到IP地址后原路返回，最后将解析的IP地址传递到发起请求的主机。DNS就是这样层次化管理的一种分布式数据库系统。

## IP地址和域名之间的转换

程序中有必要使用域名吗?

“所有学习都要在开始前认识到其必要性!”这是我经常挂在嘴边的一句话。从语言的基本语法到系统函数，若无法回答“这到底有何必要?”学习过程将变得枯燥无味，而且很容易遗忘。最头疼的是，学完之后很难应用。我们为什么需要将要讨论的转换函数?为了查看某一域名的IP地址吗?当然不是!下面通过示例解释原因。假设各位是运营www.SuperOrange.com城名的公司系统工程师，需要开发客户端使用公司提供的服务。该客户端需要接入如下服务器地址:

IP 211.102.204.12.PORT 2012

应向程序用户提供便利的运行方法，因此，程序不能像运行示例程序那样要求用户输入IP和端口信息。那该如何将上述信息传递到程序内部?难道要直接将地址信息写人程序代码吗?当然，这样便于运行程序，但这种方案也有问题。系统运行时，保持IP地址并不容易。特别是依赖ISP服务提供者维护IP地址时，系统相关的各种原因会随时导致IP地址变更。虽然ISP会保证维持原有IP，但程序不能完全依赖于这一点。万一发生地址变更，就需要向用户进行如下解释:

“请卸载当前使用的程序，到主页下载并重新安装v1.2。”

那么，因为随时可能发生地址变更，所以向用户提供源代码，每次变更地址时让用户改变IP和端口号，并重新编译程序，这又如何?

IP地址比域名发生变更的概率要高，所以利用P地址编写程序并非上策。还有什么办法呢?一日注册域名可能永久不变，因此利用域名编写程序会好一些。这样，每次运行程序时根据域名获取IP地址，再接人服务器，这样程序就不会依赖于服务器IP地址了。所以说，程序中也需要IP地址和域名之间的转换函数。

利用域名获取IP地址

使用以下函数可以通过传递字符串格式的域名获取IP地址

```c
#include <netdb.h>
struct hostent *gethostbyname(const char * hostname);
// 成功时返回hostent结构体地址，失败时返回NULL指针。

```

这个函数使用方便。只要传递域名字符串，就会返回域名对应的IP地址。只是返回时，地址信息装入hostent结构体。此结构体定义如下。

```c
struct hostent {
    char* h_name;           // 官方名称
    char** h_aliases;       // 别名列表
    int h_addrtype;         // 地址类型
    int h_length;           // 地址长度
    char** h_addr_list;     // 地址列表
};
```


从上述结构体定义中可以看出，不只返回IP信息，同时还连带着其他信息。各位不用想得太过复杂。域名转IP时只需关注h_addr_list。下面简要说明上述结构体各成员。

h_name

该变量中存有官方域名(Oficial_domain_name)。官方域名代表某一主页，但实际上，一些著名公司的域名并未用官方域名注册

h_aliases可以通过多个域名访问同一主页。同一IP可以绑定多个域名，因此，除官方域名外还可指定其他域名。这些信息可以通过haliases获得。

h_addrtype

gethostbyname函数不仅支持IPv4，还支持IPv6。因此可以通过此变量获取保存在h_addr_list的IP地址的地址族信息。若是IPv4，则此变量存有AF_INET。

h_length

保存IP地址长度。若是IPv4地址，因为是4个字节，则保存4;IPv6时，因为是16个字节，故保存16。

h_addr_list

这是最重要的成员。通过此变量以整数形式保存域名对应的IP地址。另外，用户较多的网站有可能分配多个IP给同一域名，利用多个服务器进行负载均衡。此时同样可以通过此变量获取IP地址信息。

```c++
#include <arpa/inet.h>
#include <bits/in_addr.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " hostname" << std::endl;
        return 0;
    }

    hostent* host = gethostbyname(argv[1]);
    if (host == nullptr) {
        std::cout << "gethostbyname 错误" << std::endl;
        return 0;
    }

    std::cout << "官方名称：" << host->h_name << std::endl;
    for (int i = 0; host->h_aliases[i] != nullptr; ++i) {
        std::cout << "别名" << i + 1 << "：" << host->h_aliases[i] << std::endl;
    }
    std::cout << "IP类型：" << (host->h_addrtype == AF_INET ? "IPv4" : "IPv6") << std::endl;
    for (int i = 0; host->h_addr_list[i] != nullptr; ++i) {
        std::cout << "IP" << i + 1 << "：" << inet_ntoa(*(in_addr*)host->h_addr_list[i]);
        std::cout << std::endl;
    }

    return 0;
}


```

我利用Naver网站域名运行了上述示例，大家可以任选一个域名验。现在讨论上述示例的第26~28行。若只看hostent结构体的定义，结构体成员haddr list指向字符串指针数组(由多个字符串地址构成的数组)。但字符串指针数组中的元素实际指向的是(实际保存的是)m addr结构体变量地址值而非字符串。

> 为什么是`char*`而不是`in_addr*`
> hostent结构体的成员h_addr_list指向的数组类型并不是in_addr结构体的指针数组，而是采用了char指针。各位也许对这一点感到困惑，但我认为大家应该能料到。hostent 结构体并非只为IPv4准备。h_addr_list指向的数组中也可以保存IPv6地址信息。因此，考虑到通用性，声明为char 指针类型的数组。
> “声明为void指针类型是否更合理?”
> 若能想到这一点，说明对C语言掌握非常到位。当然如此。指针对象不明确时，更适合使用void指针类型。但各位目前学习的套接字相关函数都是在void指针标准化之前定义的，而当时无法明确指出指针类型时采用的是char指针。

利用IP地址获取域名

之前介绍的gethostbyname函数利用域名获取包括IP地址在内的域相关信息。而gethostbyaddr函数利用IP地址获取域相关信息

```c
#include <netdb.h>
struct hostent* gethostbyaddr(const char * addr, socklen_t len, int family);
// 成功时返回hostent结构体变量地址值，失败时返回NULL指针。
// addr         含有IP地址信息的in_addr结构体指针。为了同时传递IPv4地址之外的其他信息，该变量的类型声明为char指针。
// len          向第一个参数传递的地址信息的字节数，IPv4时为4，IPv6时为16。
// family       传递地址族信息，IPv4时为AF_INET，IPv6时为AF_INET6。

```

如果已经彻底掌握gethostbyname函数，那么上述函数理解起来并不难。下面通过示例演示该函数的使用方法。

```c++
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <linux/in.h>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " IP" << std::endl;
        return 0;
    }

    sockaddr_in adr;
    std::memset(&adr, 0, sizeof(adr));
    adr.sin_addr.s_addr = inet_addr(argv[1]);
    hostent* host = gethostbyaddr(&adr.sin_addr, sizeof(adr.sin_addr), AF_INET);
    if (host == nullptr) {
        std::cout << "gethostbyaddr 错误" << std::endl;
        return 0;
    }

    std::cout << "官方名称：" << host->h_name << std::endl;
    for (int i = 0; host->h_aliases[i] != nullptr; ++i) {
        std::cout << "别名" << i + 1 << "：" << host->h_aliases[i] << std::endl;
    }

    std::cout << "IP类型：" << (host->h_addrtype == AF_INET ? "IPv4" : "IPv6") << std::endl;
    for (int i = 0; host->h_addr_list[i] != nullptr; ++i) {
        char buf[30] = { 0 };
        inet_ntop(AF_INET, host->h_addr_list[i], buf, 30);
        std::cout << "IP" << i + 1 << "：" << buf << std::endl;
    }

    return 0;
}


```

不知道为什么，一直报错






