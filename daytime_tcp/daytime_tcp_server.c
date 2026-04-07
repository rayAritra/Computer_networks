#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 1313
#define BUFFER_SIZE 256

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char time_str[BUFFER_SIZE];
    time_t raw_time;
    struct tm* time_info;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);
    printf("Day-Time TCP Server on port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        time(&raw_time);
        time_info = localtime(&raw_time);
        strftime(time_str, sizeof(time_str), "Date & Time: %Y-%m-%d %H:%M:%S\n", time_info);
        send(client_fd, time_str, strlen(time_str), 0);
        printf("Sent time to %s\n", inet_ntoa(client_addr.sin_addr));
        close(client_fd);
    }

    close(server_fd);
    return 0;
}