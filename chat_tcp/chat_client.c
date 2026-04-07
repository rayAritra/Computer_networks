#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9090
#define BUFFER_SIZE 1024

int sock_fd;

void* receive_thread(void* arg) {
    char buffer[BUFFER_SIZE];
    int n;
    while ((n = recv(sock_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[n] = '\0';
        printf("Server: %s", buffer);
    }
    return NULL;
}

int main() {
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    pthread_t tid;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("Connected! Start chatting:\n");

    pthread_create(&tid, NULL, receive_thread, NULL);

    while (1) {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(sock_fd, buffer, strlen(buffer), 0);
    }

    close(sock_fd);
    return 0;
}