#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("UDP Echo Server listening on port %d...\n", PORT);

    while (1) {
        int n = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
                         (struct sockaddr*)&client_addr, &addr_len);
        buffer[n] = '\0';
        printf("Received from %s: %s", inet_ntoa(client_addr.sin_addr), buffer);
        sendto(sock_fd, buffer, n, 0,
               (struct sockaddr*)&client_addr, addr_len);
    }

    close(sock_fd);
    return 0;
}