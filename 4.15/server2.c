#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUF_SIZE] = {0};
    char send_buf[BUF_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("绑定端口成功：%d\n", PORT);

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("正在监听端口：%d...\n", PORT);

    while(1) {
        printf("\n等待客户端连接...\n");

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("客户端连接成功 IP:%s, Port:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        // 轮询通信实现
        while(1) {
            int len = recv(new_socket, buffer, BUF_SIZE, 0);
            if(len <= 0){
                printf("客户端断开连接\n");
                break;
            }
            printf("客户端消息：%.*s", len, buffer);

            printf("服务器回复：");
            fgets(send_buf, BUF_SIZE, stdin);
            send(new_socket, send_buf, strlen(send_buf), 0);

            memset(buffer, 0, BUF_SIZE);
            memset(send_buf, 0, BUF_SIZE);
        }
        close(new_socket);
    }
    close(server_fd);
    return 0;
}
