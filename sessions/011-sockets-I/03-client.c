#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd < 0) {
        perror("socket");
        return 1;
    }

    printf("Socket fd: %d\n", client_fd);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    connect(client_fd, (struct sockaddr *)&addr, sizeof(addr));

    char *message = "hello from client \n";
    write(client_fd, message, strlen(message));

    char buffer[1024] = {0};
    int bytes = read(client_fd, buffer, sizeof(buffer) - 1);
    printf("recieved %d bytes from server\n", bytes);
    printf("Server said: %s\n", buffer);

    close(client_fd);

    return 0;
}
