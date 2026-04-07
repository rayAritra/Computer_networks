#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9090
#define BUFFER_SIZE 1024

int client_fd;

void* receive_thread(void* arg) {
    char buffer[BUFFER_SIZE];
    int n;
    while ((n = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[n] = '\0';
        printf("Client: %s", buffer);
    }
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    pthread_t tid;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);
    printf("Chat Server started on port %d. Waiting for client...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    printf("Client connected! Start chatting:\n");

    pthread_create(&tid, NULL, receive_thread, NULL);

    while (1) {
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_fd, buffer, strlen(buffer), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}