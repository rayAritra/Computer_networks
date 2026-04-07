#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1313
#define BUFFER_SIZE 256

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    int n = recv(sock_fd, buffer, BUFFER_SIZE, 0);
    buffer[n] = '\0';
    printf("Received from server: %s", buffer);

    close(sock_fd);
    return 0;
}