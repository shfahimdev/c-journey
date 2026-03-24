# Session 011 — Sockets I (TCP Basics)

**Date:** 2026-03-24
**Duration:** ~1 session
**Folder:** `011-sockets-1/`

---

## What We Covered

### What is a Socket?
- A socket is just a file descriptor — the same kind `open()` gives you for a file
- You `read()` and `write()` on it exactly like a file — the difference is the other end is across a network
- `socket()` returns an integer fd — 0, 1, 2 are stdin/stdout/stderr, so your socket gets 3

### The Server Lifecycle
Six functions, always in this order:
```
socket()  → create the fd
bind()    → attach it to an address + port
listen()  → mark it as passive (waiting for connections)
accept()  → block until a client connects, returns a NEW fd
read()    → receive bytes from the client
write()   → send bytes back
close()   → clean up both fds
```

### socket()
```c
int server_fd = socket(AF_INET, SOCK_STREAM, 0);
```
- `AF_INET` — IPv4 address family
- `SOCK_STREAM` — TCP (reliable, ordered, connection-based)
- `0` — let the OS pick the default protocol for this type

### sockaddr_in
```c
struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr));       // zero out before filling
addr.sin_family      = AF_INET;
addr.sin_port        = htons(8080);   // host to network byte order
addr.sin_addr.s_addr = INADDR_ANY;    // accept on any interface
```
- Always `memset` to zero first — clears padding bytes
- `htons` — flips byte order from little-endian (CPU) to big-endian (network)
- `INADDR_ANY` — `0.0.0.0`, accept connections on any network interface

### bind()
```c
bind(server_fd, (struct sockaddr *)&addr, sizeof(addr))
```
- Attaches the socket to port 8080 on this machine
- Cast to `struct sockaddr *` — bind takes a generic pointer, works for all address families
- The OS knows the real type from `sin_family`

### listen()
```c
listen(server_fd, 10);
```
- Marks socket as passive — ready to receive incoming connections
- Second arg is backlog — how many pending connections to queue before refusing

### accept()
```c
int client_fd = accept(server_fd, NULL, NULL);
```
- Blocks until a client connects
- Returns a **new** fd for that specific client — never read/write on `server_fd`
- `server_fd` keeps listening, `client_fd` is the actual conversation

### read() / write()
```c
char buffer[1024] = {0};
int bytes = read(client_fd, buffer, sizeof(buffer) - 1);
write(client_fd, response, strlen(response));
```
- Identical to file I/O — same functions, same signature
- `sizeof(buffer) - 1` leaves room for null terminator

### SO_REUSEADDR
```c
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```
- Add this right after every `socket()` call — no exceptions
- Without it, restarting your server after a crash blocks on "address already in use"
- The OS holds ports in TIME_WAIT after close — this bypasses that

### The Client Side
Client uses `connect()` instead of `bind/listen/accept`:
```c
int client_fd = socket(AF_INET, SOCK_STREAM, 0);

struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_port   = htons(8080);
inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

connect(client_fd, (struct sockaddr *)&addr, sizeof(addr));
```
- `inet_pton` — Presentation To Network — converts `"127.0.0.1"` string to binary
- No `bind`, no `listen`, no `accept` — those are server-only

### Minimum Valid HTTP Response
```c
char *response = "HTTP/1.1 200 OK\r\n\r\nHello from Fahim's Server\n";
```
- Status line + blank line = curl accepts it without complaint
- `\r\n` is the HTTP line terminator
- `\r\n\r\n` signals end of headers

---

## Gotchas Hit
- `read()` and `write()` need `#include <unistd.h>` — not in `arpa/inet.h`
- "address already in use" — port stuck in TIME_WAIT after server exits
  - Fix: `SO_REUSEADDR` in every server, `fuser -k 8080/tcp` to clear it manually
- `setsockopt` called before checking `server_fd < 0` — always error check socket() first
- `int opt = 1` declared but `setsockopt` not called — useless without the call

---

## Files Written
- `01-socket.c` — socket, bind, listen, accept, read, write, close
- `02-echo.c` — same but echoes client bytes back instead of hardcoded response
- `03-client.c` — client side: socket, connect, write, read
- `04-curl.c` — server that responds with valid HTTP, curl accepts without --http0.9

---

## Observed in Terminal
- `server_fd = 3`, `client_fd = 4` — fds are just the next available integers after 0/1/2
- Raw HTTP request printed: `GET / HTTP/1.1` + headers — exactly what the HTTP server will parse
- Client and server talking to each other — two programs you wrote, communicating over TCP
- `curl http://localhost:8080` returned "Hello from Fahim's Server" with no flags needed

---

## Why This Matters for the HTTP Server
This is the literal foundation. Every HTTP connection starts with `accept()`. The request
is `read()`. The response is `write()`. Sessions 016-021 are entirely built on these 6 functions.

---

## Where We Stopped
Finished `011-sockets-1`. All 4 exercises done.

## What's Next
- **Sockets II** — SO_REUSEADDR deep dive, partial reads, SIGPIPE, graceful shutdown
