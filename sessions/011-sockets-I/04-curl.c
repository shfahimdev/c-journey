#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    printf("Socket fd: %d\n", server_fd);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    printf("bound to port 8080\n");

    listen(server_fd, 10);
    printf("Listening to port 8080...\n");

    int client_fd = accept(server_fd, NULL, NULL);
    printf("client connected, client_fd: %d\n", client_fd);

    char buffer[1024] = {0};
    int bytes = read(client_fd, buffer, sizeof(buffer) - 1);
    printf("recieved %d bytes\n%s\n", bytes, buffer);

    char *response = "HTTP/1.1 200 OK\r\n\r\nHello from Fahim's Server\n";
    write(client_fd, response, strlen(response));

    close(client_fd);
    close(server_fd);

    return 0;
}
