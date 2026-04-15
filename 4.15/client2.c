#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUF_SIZE 1024

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE] = {0};
    char send_buf[BUF_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("地址错误或者地址不支持\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("连接失败\n");
        return -1;
    }

    printf("连接到服务器 %s:%d\n", SERVER_IP, SERVER_PORT);
    printf("可以发送信息 (Press Ctrl+C to exit):\n");

    // 轮询通信实现
    while(1) {
        printf("客户端发送：");
        fgets(send_buf, BUF_SIZE, stdin);
        send(sock, send_buf, strlen(send_buf), 0);

        int len = recv(sock, buffer, BUF_SIZE, 0);
        if(len <= 0){
            printf("服务器断开连接\n");
            break;
        }
        printf("服务器回复：%.*s", len, buffer);
        memset(buffer, 0, BUF_SIZE);
        memset(send_buf, 0, BUF_SIZE);
    }

    close(sock);
    return 0;
}
