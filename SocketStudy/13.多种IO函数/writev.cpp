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
