#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 1314
#define BUFFER_SIZE 256

int main() {
    int sock_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE], time_str[BUFFER_SIZE];

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("Day-Time UDP Server on port %d...\n", PORT);

    while (1) {
        recvfrom(sock_fd, buffer, BUFFER_SIZE, 0,
                 (struct sockaddr*)&client_addr, &addr_len);
        time_t raw = time(NULL);
        strftime(time_str, sizeof(time_str),
                 "Date & Time: %Y-%m-%d %H:%M:%S\n", localtime(&raw));
        sendto(sock_fd, time_str, strlen(time_str), 0,
               (struct sockaddr*)&client_addr, addr_len);
    }

    close(sock_fd);
    return 0;
}