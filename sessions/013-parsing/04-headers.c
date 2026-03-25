#include <stdio.h>
#include <string.h>

#define MAX_HEADERS 10

typedef struct {
    char key[64];
    char value[128];
} Header;

int main() {
    char raw[] = "Host: localhost\r\nContent-Type: text/html\r\nContent-Length: 42\r\n";

    Header headers[MAX_HEADERS];
    int count = 0;

    char *line = strtok(raw, "\r\n");
    while (line != NULL && count < MAX_HEADERS) {
        int parsed = sscanf(line, "%63[^:]: %127[^\r]", headers[count].key, headers[count].value);

        if (parsed == 2) {
            count++;
        }

        line = strtok(NULL, "\r\n");
    }

    for (int i = 0; i < count; i++) {
        printf("[%s] -> [%s]\n", headers[i].key, headers[i].value);
    }

    return 0;
}
