#include <stdio.h>
#include <string.h>

#define MAX_HEADERS 10

typedef struct {
    char key[64];
    char value[128];
} Header;

typedef struct {
    char method[8];
    char path[256];
    char version[16];
    Header headers[MAX_HEADERS];
    int header_count;
    char *body;
} Request;

void parse_request(char *raw, Request *req) {
    // 1. find the end of headers
    char *body = strstr(raw, "\r\n\r\n");
    if (body != NULL) {
        req->body = body + 4;
    } else {
        req->body = NULL;
    }

    // 2. parse request lines
    char method[8], path[256], version[16];
    int parsed = sscanf(raw, "%7s %255s %15s", method, path, version);
    if (parsed != 3)
        return;

    strncpy(req->method, method, sizeof(req->method) - 1);
    strncpy(req->path, path, sizeof(req->path) - 1);
    strncpy(req->version, version, sizeof(req->version) - 1);

    // 3. parse headers
    char *line = strtok(raw, "\r\n");
    req->header_count = 0;
    while (line != NULL && req->header_count < MAX_HEADERS) {
        int parsed = sscanf(line, "%63[^:]: %127[^\r]", req->headers[req->header_count].key,
                            req->headers[req->header_count].value);

        if (parsed == 2) {
            req->header_count++;
        }
        line = strtok(NULL, "\r\n");
    }
}

int main() {
    char raw[] = "GET /index.html HTTP/1.1\r\n"
                 "Host: localhost\r\n"
                 "Content-Length: 11\r\n"
                 "\r\n"
                 "Hello World";

    Request req = {0};
    parse_request(raw, &req);

    printf("method  : %s\n", req.method);
    printf("path    : %s\n", req.path);
    printf("version : %s\n", req.version);
    printf("headers : %d\n", req.header_count);
    for (int i = 0; i < req.header_count; i++) {
        printf("  [%s] -> [%s]\n", req.headers[i].key, req.headers[i].value);
    }
    if (req.body != NULL) {
        printf("body    : %s\n", req.body);
    }

    return 0;
}
