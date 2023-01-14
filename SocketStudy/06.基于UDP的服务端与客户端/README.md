# 基于UDP的服务器端客户端

## 理解UDP

我们在第4章学习TCP的过程中，还同时了解了TCP/IP协议栈。在4层TCP/IP模型中，上数第二层传输（Transport）层分为TCP和UDP这2种。数据交换过程可以分为通过TCP套接字完成的TCP 方式和通过UDP套接字完成的UDP方式。

**UDP 套接字的特点**

下面通过信件说明UDP的工作原理，这是讲解UDP时使用的传统示例，它与UDP特性完全相符。寄信前应先在信封上填好寄信人和收信人的地址，之后贴上邮票放进邮筒即可。当然，信件的特点使我们无法确认对方是否收到。另外，邮寄过程中也可能发生信件丢失的情况。也就是说，信件是一种不可靠的传输方式。与之类似，UDP提供的同样是不可靠的数据传输服务。

“既然如此，TCP应该是更优质的协议吧？”

如果只考虑可靠性，TCP的确比UDP好。但UDP在结构上比TCP更简洁。UDP不会发送类似ACK的应答消息，也不会像SEQ那样给数据包分配序号。因此，UDP的性能有时比TCP高出很多。编程中实现UDP也比TCP简单。另外，UDP的可靠性虽比不上TCP，但也不会像想象中那么频繁地发生数据损毁。因此，在更重视性能而非可靠性的情况下，UDP是一种很好的选择。

既然如此，UDP的作用到底是什么呢？为了提供可靠的数据传输服务，TCP在不可靠的IP层进行流控制，而UDP就缺少这种流控制机制。

"UDP和TCP的差异只在于流控制机制吗？"

是的，流控制是区分UDP和TCP的最重要的标志。但若从TCP中除去流控制，所剩内容也屈指可数。也就是说，TCP的生命在于流控制。第5章讲过的"与对方套接字连接及断开连接过程"也属于流控制的一部分。

> 虽然电话比信件要快，但是……
>
> 我把TCP比喻为电话，把UDP比喻为信件。但这只是形容协议工作方式，并没有包含数据交换速率。请不要误认为"电话的速度比信件快，因此TCP 的数据收发速率也比 UDP 快"。实际上正好相反。TCP 的速度无法超过UDP，但在收发某些类型的数据时有可能接近UDP。例如，每次交换的数据量越大，TCP的传输速率就越接近UDP的传输速率。

**UDP内部工作原理**

与TCP不同，UDP不会进行流控制。接下来具体讨论UDP的作用

IP的作用就是让离开主机B的UDP数据包准确传递到主机A。但把UDP包最终交给主机A的某一UDP套接字的过程则是由UDP完成的。UDP最重要的作用就是根据端口号将传到主机的数据包交付给最终的UDP套接字。

**UDP的高效使用**

虽然貌似大部分网络编程都基于TCP实现，但也有一些是基于UDP实现的。接下来考虑何时使用UDP更有效。讲解前希望各位明白，UDP也具有一定的可靠性。网络传输特性导致信息丢失频发，可若要传递压缩文件（发送1万个数据包时，只要丢失1个就会产生问题），则必须使用TCP，因为压缩文件只要丢失一部分就很难解压。但通过网络实时传输视频或音频时的情况有所不同。对于多媒体数据而言，丢失一部分也没有太大问题，这只会引起短暂的画面抖动，或出现细微的杂音。但因为需要提供实时服务，速度就成为非常重要的因素。因此，第5章的流控制就显得有些多余，此时需要考虑使用UDP。但UDP并非每次都快于TCP，TCP比UDP慢的原因通常有以下两点。

- 收发数据前后进行的连接设置及清除过程。
- 收发数据过程中为保证可靠性而添加的流控制。

如果收发的数据量小但需要频繁连接时，UDP比TCP更高效。有机会的话，希望各位深入学习TCP/IP协议的内部构造。C语言程序员懂得计算机结构和操作系统知识就能写出更好的程序，同样，网络程序员若能深入理解TCP/IP协议则可大幅提高自身实力。

## 实现基于UDP的服务器端/客户端

接下来通过之前介绍的UDP理论实现真正的程序。对于UDP而言，只要能理解之前的内容，实现并非难事。

**UDP中的服务器端和客户端没有连接**

UDP服务器端I客户端不像TCP那样在连接状态下交换数据，因此与TCP不同，无需经过连接过程。也就是说，不必调用TCP连接过程中调用的listen函数和accept函数。UDP中只有创建套接字的过程和数据交换过程。

**UDP服务器端和客户端均只需1个套接字**

TCP中，套接字之间应该是一对一的关系。若要向10个客户端提供服务，则除了守门的服务器套接字外，还需要10个服务器端套接字。但在UDP中，不管是服务器端还是客户端都只需要1 个套接字。之前解释UDP原理时举了信件的例子，收发信件时使用的邮简可以比喻为UDP套接字。只要附近有1个邮简，就可以通过它向任意地址寄出信件。同样，只需1个UDP套接字就可以向任意主机传输数据。

也就是说，只需1个UDP套接字就能和多台主机通信。

**基于UDP的数据I/O函数**

创建好TCP套接字后，传输数据时无需再添加地址信息。因为TCP套接字将保持与对方套接字的连接。换言之，TCP套接字知道目标地址信息。但UDP套接字不会保持连接状态（UDP套接字只有简单的邮简功能），因此每次传输数据都要添加目标地址信息。这相当于寄信前在信件中填写地址。接下来介绍填写地址并传输数据时调用的UDP相关函数。

```c
#include <sys/socket.h>
ssize_t sendto(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *to, socklen_t addrlen);
// 成功时返回传输的字节数，失败时返回-1。

// sock         用于传输数据的UDP套接字文件描述符。
// buff         保存待传输数据的缓冲地址值。
// nbytes       待传输的数据长度，以字节为单位。
// flags        可选项参数，若没有则传递0。
// to           存有目标地址信息的sockaddr结构体变量的地址值。
// addrlen      传递给参数to的地址值结构体变量长度。

```

上述函数与之前的TCP输出函数最大的区别在于，此函数需要向它传递目标地址信息。接下来介绍接收UDP数据的函数。UDP数据的发送端并不固定，因此该函数定义为可接收发送端信息的形式，也就是将同时返回UDP数据包中的发送端信息。

```c
#include <sys/socket.h>
ssize_t recvfrom(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen);
// 成功时返回接收的字节数，失败时返回-1。

// sock         用于接收数据的UDP套接字文件描述符。
// buff         保存接收数据的缓冲地址值。
// nbytes       可接收的最大字节数，故无法超过参数buff所指的缓冲大小。
// flags        可选项参数，若没有则传入0。
// from         存有发送端地址信息的sockaddr结构体变量的地址值。
// addrlen      保存参数from的结构体变量长度的变量地址值。

```

编写UDP程序时最核心的部分就在于上述两个函数，这也说明二者在UDP数据传输中的地位。

**基于UDP的回声服务器端/客户端**

下面结合之前的内容实现回声服务器。需要注意的是，UDP不同于TCP，不存在请求连接和受理过程，因此在某种意义上无法明确区分服务器端和客户端。只是因其提供服务而称为服务器端，希望各位不要误解。

```c++
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 1024;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << std::endl;
        return 0;
    }

    // 创建UDP套接字
    int servSock = socket(PF_INET, SOCK_DGRAM, 0);
    if (servSock == -1) {
        std::cout << "socket 错误" << std::endl;
        return 0;
    }

    // 初始化UDP服务端地址信息
    sockaddr_in servAdr;
    std::memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(std::atoi(argv[1]));

    // 绑定UDP地址信息
    int stu = bind(servSock, (sockaddr*)&servAdr, sizeof(servAdr));
    if (stu == -1) {
        close(servSock);
        std::cout << "bind 错误" << std::endl;
        return 0;
    }

    char msg[BUF_SIZE] = { 0 };

    while (true) {
        sockaddr_in clntAdr;
        socklen_t szClntAdr = sizeof(clntAdr);
        int strLen = recvfrom(servSock, msg, BUF_SIZE, 0, (sockaddr*)&clntAdr, &szClntAdr);
        if (strLen == -1) {
            std::cout << "recvfrom 错误" << std::endl;
            continue;
        }
        std::string adrStr = inet_ntoa(clntAdr.sin_addr);
        msg[strLen] = 0;
        std::cout << "收到客户端" << adrStr << "的消息：" << msg << std::endl;
        sendto(servSock, msg, strLen, 0, (sockaddr*)&clntAdr, szClntAdr);
    }

    close(servSock);

    return 0;
}


```

接下来介绍与上述服务器端协同工作的客户端。这部分代码与TCP客户端不同，不存在connect函数调用。

```c++
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

constexpr size_t BUF_SIZE = 1024;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " IP port" << std::endl;
        return 0;
    }

    // 创建UDP套接字
    int clntSock = socket(PF_INET, SOCK_DGRAM, 0);
    if (clntSock == -1) {
        std::cout << "socket 错误" << std::endl;
        return 0;
    }

    // 初始化目标服务端地址信息
    sockaddr_in servAdr;
    std::memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);
    servAdr.sin_port = htons(std::atoi(argv[2]));

    char buf[BUF_SIZE] = { 0 };
    // 不用连接，直接发送消息
    while (true) {
        std::cout << "输入信息：";
        std::string msg;
        std::cin >> msg;
        if (msg == "Q" || msg == "q") {
            break;
        }

        sendto(clntSock, msg.c_str(), msg.size(), 0, (sockaddr*)&servAdr, sizeof(servAdr));
        socklen_t szAdr = sizeof(servAdr);
        int strLen = recvfrom(clntSock, buf, BUF_SIZE, 0, (sockaddr*)&servAdr, &szAdr);
        buf[strLen] = 0;
        std::string adrStr = inet_ntoa(servAdr.sin_addr);
        std::cout << "从" << adrStr << "收到消息：" << buf << std::endl;
    }

    close(clntSock);

    return 0;
}


```

若各位很好地理解了第4章的connect函数，那么读上述代码时应有如下疑问：

"TCP客户端套接字在调用connect函数时自动分配IP地址和端口号，既然如此，UDP 客户端何时分配IP地址和端口号？"

所有套接字都应分配IP地址和端口，问题是直接分配还是自动分配。

运行过程中的顺序并不重要。只需保证在调用sendto函数前，sendto函数的目标主机程序已经开始运行。

UDP客户端套接字的地址分配

前面讲解了UDP服务器端/客户端的实现方法。但如果仔细观察UDP客户端会发现，它缺少把IP和端口分配给套接字的过程。TCP客户端调用connect函数自动完成此过程，而UDP中连能承担相同功能的函数调用语句都没有。究竟在何时分配IP和端口号呢？

UDP程序中，调用sendto函数传输数据前应完成对套接字的地址分配工作，因此调用bind函数。当然，bind函数在TCP程序中出现过，但bind函数不区分TCP和UDP，也就是说，在UDP程序中同样可以调用。另外，如果调用sendto函数时发现尚未分配地址信息，则在首次调用sendto 函数时给相应套接字自动分配IP和端口。而且此时分配的地址一直保留到程序结束为止，因此也可用来与其他UDP套接字进行数据交换。当然，IP用主机IP，端口号选尚未使用的任意端口号。

综上所述，调用sendto函数时自动分配IP和端口号，因此，UDP客户端中通常无需额外的地址分配过程。所以之前示例中省略了该过程，这也是普遍的实现方式。

## UDP的数据传输特性和调用connect函数

我们之前通过示例验证了TCP传输的数据不存在数据边界，本节将验证UDP数据传输中存在数据边界。最后讨论UDP中connect函数的调用，以此结束UDP相关讨论。

存在数据边界的UDP套接字

前面说过TCP数据传输中不存在边界，这表示"数据传输过程中调用I/O函数的次数不具有任何意义。”

相反，UDP是具有数据边界的协议，传输中调用I/O函数的次数非常重要。因此，输入函数的调用次数应和输出函数的调用次数完全一致，这样才能保证接收全部已发送数据。例如，调用3次输出函数发送的数据必须通过调用3次输入函数才能接收完。

已连接（connected）UDP套接字与未连接（unconnected）UDP套接字

TCP套接字中需注册待传输数据的目标IP和端口号，而UDP中则无需注册。因此，通过sendto 函数传输数据的过程大致可分为以下3个阶段。

- 第1阶段：向UDP套接字注册目标IP和端口号。
- 第2阶段：传输数据。
- 第3阶段：删除UDP套接字中注册的目标地址信息。

每次调用sendto函数时重复上述过程。每次都变更目标地址，因此可以重复利用同一UDP套接字向不同目标传输数据。这种未注册目标地址信息的套接字称为未连接套接字，反之，注册了目标地址的套接字称为连接connected套接字。显然，UDP套接字默认属于未连接套接字。但UDP 套接字在下述情况下显得不太合理：

"IP为211.210.147.82的主机82号端口共准备了3个数据，调用3次sendto函数进行传输。"

此时需重复3次上述三阶段。因此，要与同一主机进行长时间通信时，将UDP套接字变成已连接套接字会提高效率。上述三个阶段中，第一个和第三个阶段占整个通信过程近1/3的时间，缩短这部分时间将大大提高整体性能。

创建已连接UDP套接字的过程格外简单，只需针对UDP套接字调用connect函数。

之后就与TCP套接字一样，每次调用sendto函数时只需传输数据。因为已经指定了收发对象，所以不仅可以使用sendto、recvfrom函数，还可以使用write、read函数进行通信。




