#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    std::cout << "Socket客户端" << std::endl;

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == client_fd) {
        std::cout << "socket()错误" << std::endl;
        return 0;
    }

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8848);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int rtn_status = connect(client_fd, (sockaddr*)&server_addr, sizeof(server_addr));
    if (0 > rtn_status) {
        std::cout << "connect()错误" << std::endl;
        return 0;
    }

    std::string str = "Linux socket test 网络测试";
    size_t data_len = str.size();
    rtn_status = send(client_fd, &data_len, sizeof(data_len), 0);
    rtn_status = send(client_fd, str.c_str(), str.length(), 0);
    if (0 > rtn_status) {
        std::cout << "send()错误" << std::endl;
        close(client_fd);
        return 0;
    }

    char buffer[1024] = { '\0' };
    int num_recv = recv(client_fd, buffer, sizeof(buffer), 0);
    if (num_recv > 0) {
        str.clear();
        str.append(buffer, num_recv);
        std::cout << str << std::endl;
    }

    close(client_fd);

    return 0;
}
