#include <stdio.h>
#include <string.h>

int main() {
    char request[] = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\nHello body";

    char *end = strstr(request, "\r\n\r\n");

    if (end == NULL) {
        printf("No header found\n");
        return 1;
    }

    printf("found at offset%ld\n", end - request);
    printf("body starts at %s\n", end + 4);

    return 0;
}
