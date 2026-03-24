# Cheatsheet — 11 Sockets I

## Headers
```c
#include <arpa/inet.h>   // socket, bind, listen, accept, sockaddr_in, htons, inet_pton
#include <unistd.h>      // read, write, close
#include <string.h>      // memset
#include <stdio.h>       // perror, printf
```

---

## Server Skeleton
```c
// 1. create socket
int server_fd = socket(AF_INET, SOCK_STREAM, 0);
if (server_fd < 0) { perror("socket"); return 1; }

// 2. always add this — prevents "address already in use" on restart
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

// 3. fill address struct
struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr));
addr.sin_family      = AF_INET;
addr.sin_port        = htons(8080);
addr.sin_addr.s_addr = INADDR_ANY;

// 4. bind
if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind"); return 1;
}

// 5. listen
listen(server_fd, 10);

// 6. accept — blocks until client connects
int client_fd = accept(server_fd, NULL, NULL);

// 7. read / write
char buffer[1024] = {0};
int bytes = read(client_fd, buffer, sizeof(buffer) - 1);
write(client_fd, buffer, bytes);

// 8. close both
close(client_fd);
close(server_fd);
```

---

## Client Skeleton
```c
int client_fd = socket(AF_INET, SOCK_STREAM, 0);
if (client_fd < 0) { perror("socket"); return 1; }

struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_port   = htons(8080);
inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

connect(client_fd, (struct sockaddr *)&addr, sizeof(addr));

write(client_fd, "hello\n", 6);

char buffer[1024] = {0};
read(client_fd, buffer, sizeof(buffer) - 1);
printf("%s\n", buffer);

close(client_fd);
```

---

## Key Concepts

| Concept | Detail |
|---------|--------|
| Socket fd | Just an integer — same as a file fd |
| `AF_INET` | IPv4 address family |
| `SOCK_STREAM` | TCP — reliable, ordered |
| `htons(port)` | Flip bytes to network order — wrap every port number in this |
| `INADDR_ANY` | `0.0.0.0` — accept on all interfaces |
| `inet_pton` | String IP `"127.0.0.1"` → binary |
| `accept()` | Returns a NEW fd — never read/write on server_fd |
| `SO_REUSEADDR` | Add to every server — prevents TIME_WAIT block on restart |

---

## Server vs Client
| Step | Server | Client |
|------|--------|--------|
| Create | `socket()` | `socket()` |
| Setup | `bind()` → `listen()` → `accept()` | `connect()` |
| Talk | `read()` / `write()` | `read()` / `write()` |
| Done | `close()` both fds | `close()` |

---

## Minimum Valid HTTP Response
```c
// curl accepts this without --http0.9
char *response = "HTTP/1.1 200 OK\r\n\r\nHello World\n";
write(client_fd, response, strlen(response));
```
- `\r\n` — HTTP line terminator
- `\r\n\r\n` — blank line signals end of headers

---

## Useful Commands
```bash
fuser -k 8080/tcp       # kill whatever holds port 8080
curl http://localhost:8080
curl -v http://localhost:8080    # verbose — shows request and response headers
curl --http0.9 http://localhost:8080   # accept raw non-HTTP response
```

---

## Gotchas
- Missing `SO_REUSEADDR` → "address already in use" on every restart
- `read()`/`write()`/`close()` need `unistd.h` — not `arpa/inet.h`
- Always `memset(&addr, 0, sizeof(addr))` before filling the struct
- `accept()` returns a new fd — read/write on that, not on `server_fd`
- Check `socket() < 0` before calling `setsockopt`
