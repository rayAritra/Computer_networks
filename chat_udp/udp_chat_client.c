#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9090
#define BUFFER_SIZE 1024

int sock_fd;
struct sockaddr_in server_addr;
socklen_t addr_len;

void* recv_thread(void* arg) {
    char buffer[BUFFER_SIZE];
    struct sockaddr_in from;
    socklen_t from_len = sizeof(from);
    while (1) {
        int n = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
                         (struct sockaddr*)&from, &from_len);
        buffer[n] = '\0';
        printf("Server: %s", buffer);
    }
    return NULL;
}

int main() {
    char buffer[BUFFER_SIZE];
    pthread_t tid;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    addr_len = sizeof(server_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    pthread_create(&tid, NULL, recv_thread, NULL);
    printf("UDP Chat Client started. Type messages:\n");

    while (1) {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        sendto(sock_fd, buffer, strlen(buffer), 0,
               (struct sockaddr*)&server_addr, addr_len);
    }

    close(sock_fd);
    return 0;
}
