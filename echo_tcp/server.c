#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket failed"); exit(1); }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("TCP Echo Server listening on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

    int n;
    while ((n = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[n] = '\0';
        printf("Received: %s", buffer);
        send(client_fd, buffer, n, 0);  // Echo back
    }

    close(client_fd);
    close(server_fd);
    return 0;
}