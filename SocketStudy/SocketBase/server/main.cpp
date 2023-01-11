#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    std::cout << "Socket服务端" << std::endl;

    int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == serv_fd) {
        std::cout << "socket()错误" << std::endl;
        return 0;
    }

    sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8848);

    int rtn_status = bind(serv_fd, (sockaddr*)&servaddr, sizeof(servaddr));
    if (0 != rtn_status) {
        std::cout << "bind()错误" << std::endl;
        return 0;
    }

    rtn_status = listen(serv_fd, 5);
    if (0 != rtn_status) {
        std::cout << "listen()错误" << std::endl;
        return 0;
    }

    bool run_flag = true;
    while (run_flag) {
        sockaddr_in client_addr;
        size_t addr_len = sizeof(client_addr);
        int client_fd = accept(serv_fd, (sockaddr*)&client_addr, (socklen_t*)&addr_len);
        if (-1 == client_fd) {
            std::cout << "accept()错误" << std::endl;
            break;
        }

        char buffer[1024] = { '\0' };
        std::string str;
        std::cout << "开始接收数据" << std::endl;
        size_t data_len = 0;
        recv(client_fd, &data_len, sizeof(data_len), 0);
        while (data_len > 0) {
            std::memset(buffer, 0, sizeof(buffer));
            int num_recv = recv(client_fd, buffer, sizeof(buffer), 0);
            data_len -= num_recv;
            if (num_recv == 0) {
                std::cout << "连接断开" << std::endl;
                break;
            } else if (num_recv < 0) {
                std::cout << "连接错误" << std::endl;
                break;
            } else {
                str.append(buffer, num_recv);
            }
        }
        close(client_fd);
        std::cout << str << std::endl;
        run_flag = false;
    }
    close(serv_fd);

    return 0;
}
