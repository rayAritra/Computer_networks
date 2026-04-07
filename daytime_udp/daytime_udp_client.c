#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1314
#define BUFFER_SIZE 256

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);
    char buffer[BUFFER_SIZE] = "REQUEST";

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    sendto(sock_fd, buffer, strlen(buffer), 0,
           (struct sockaddr*)&server_addr, sizeof(server_addr));

    int n = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
                     (struct sockaddr*)&server_addr, &addr_len);
    buffer[n] = '\0';
    printf("Server says: %s", buffer);

    close(sock_fd);
    return 0;
}