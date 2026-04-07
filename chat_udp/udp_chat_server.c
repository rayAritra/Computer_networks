#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9090
#define BUFFER_SIZE 1024

int sock_fd;
struct sockaddr_in client_addr;
socklen_t addr_len;

void* recv_thread(void* arg) {
    char buffer[BUFFER_SIZE];
    struct sockaddr_in from;
    socklen_t from_len = sizeof(from);
    while (1) {
        int n = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
                         (struct sockaddr*)&from, &from_len);
        buffer[n] = '\0';
        client_addr = from;
        addr_len = from_len;
        printf("Client: %s", buffer);
    }
    return NULL;
}

int main() {
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    pthread_t tid;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("UDP Chat Server on port %d. Waiting...\n", PORT);

    pthread_create(&tid, NULL, recv_thread, NULL);

    while (1) {
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        if (addr_len > 0)
            sendto(sock_fd, buffer, strlen(buffer), 0,
                   (struct sockaddr*)&client_addr, addr_len);
    }

    close(sock_fd);
    return 0;
}