#include <stdio.h>
#include <string.h>

int main() {
    char line[] = "GET /index.html HTTP/1.1\r\n";
    // char line[] = "GARBAGE";

    char *method = strtok(line, " ");
    char *path = strtok(NULL, " ");
    char *version = strtok(NULL, "\r");

    if (method == NULL || path == NULL || version == NULL) {
        printf("Malformed HTTP request\n");
        return 1;
    }

    printf("method      :%s\n", method);
    printf("path        :%s\n", path);
    printf("version     :%s\n", version);

    return 0;
}
