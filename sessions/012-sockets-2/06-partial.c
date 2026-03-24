#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define MSG_SIZE 64

int main() {

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        return 1;
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        return 1;
    listen(server_fd, 10);
    printf("listening to port %d...\n", PORT);

    int client_fd = accept(server_fd, NULL, NULL);

    char buffer[MSG_SIZE];
    int total = 0;
    while (total < MSG_SIZE) {
        int n = read(client_fd, buffer + total, MSG_SIZE - total);
        if (n <= 0)
            break;
        total += n;
        printf("got %d bytes (total: %d/%d)\n", n, total, MSG_SIZE);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
