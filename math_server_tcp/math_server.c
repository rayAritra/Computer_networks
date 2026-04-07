#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7070
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE], result[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);
    printf("Math Server on port %d. Format: NUM OP NUM (e.g. 10 + 5)\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        int n = recv(client_fd, buffer, BUFFER_SIZE, 0);
        buffer[n] = '\0';

        double a, b, res;
        char op;
        sscanf(buffer, "%lf %c %lf", &a, &op, &b);

        switch (op) {
            case '+': res = a + b; break;
            case '-': res = a - b; break;
            case '*': res = a * b; break;
            case '/':
                if (b != 0) res = a / b;
                else { send(client_fd, "Error: Division by zero\n", 24, 0); close(client_fd); continue; }
                break;
            default:
                send(client_fd, "Error: Unknown operator\n", 24, 0);
                close(client_fd);
                continue;
        }

        snprintf(result, sizeof(result), "Result: %.2lf %c %.2lf = %.2lf\n", a, op, b, res);
        send(client_fd, result, strlen(result), 0);
        printf("Processed: %s", result);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}