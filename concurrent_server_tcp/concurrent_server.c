#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 6060
#define BUFFER_SIZE 1024

void* handle_client(void* arg) {
    int client_fd = *(int*)arg;
    free(arg);
    char buffer[BUFFER_SIZE];
    int n;

    printf("New client connected (fd=%d)\n", client_fd);
    char welcome[] = "Welcome to Concurrent TCP Server!\n";
    send(client_fd, welcome, strlen(welcome), 0);

    while ((n = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[n] = '\0';
        printf("[Client %d]: %s", client_fd, buffer);
        // Echo back with prefix
        char response[BUFFER_SIZE + 50];
        snprintf(response, sizeof(response), "[Server->%d]: %s", client_fd, buffer);
        send(client_fd, response, strlen(response), 0);
    }

    printf("Client %d disconnected.\n", client_fd);
    close(client_fd);
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 10);
    printf("Concurrent TCP Server on port %d. Accepting multiple clients...\n", PORT);

    while (1) {
        int* client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_fd);
        pthread_detach(tid);  // Auto-cleanup when done
    }

    close(server_fd);
    return 0;
}