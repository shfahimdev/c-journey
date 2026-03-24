# Cheatsheet — 12 Sockets II

## SO_REUSEADDR
```c
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```
- Add after every `socket()` call — prevents "address already in use" on restart
- The OS holds ports in TIME_WAIT after close — this bypasses that

---

## Partial Read Loop
```c
char buffer[EXPECTED];
int total = 0;
while (total < EXPECTED) {
    int n = read(client_fd, buffer + total, EXPECTED - total);
    if (n <= 0) break;   // 0 = disconnected, -1 = error
    total += n;
}
```
- `buffer + total` — write into the next free slot, not the start
- `EXPECTED - total` — only ask for what's still missing
- `n <= 0` — covers both clean disconnect and error

---

## SIGPIPE
```c
signal(SIGPIPE, SIG_IGN);
```
- Without this, writing to a closed socket kills your server silently
- Ignore it — handle write errors via return values instead
- Add this near the top of `main()`, before any socket work

---

## Graceful Shutdown
```c
#include <signal.h>

int server_fd;   // globals so the handler can reach them
int client_fd;

void sigint_handler(int sig) {
    close(server_fd);
    close(client_fd);
    _exit(0);   // not exit(), not return — _exit()
}

// in main:
signal(SIGINT, sigint_handler);
signal(SIGPIPE, SIG_IGN);
```

---

## Gotchas
| Mistake | Fix |
|---------|-----|
| `return` in signal handler | use `_exit(0)` — return just resumes interrupted code |
| Custom `SIGPIPE` handler that exits | use `SIG_IGN` — keep running, check `write()` return value |
| Single `read()` and assume done | always loop until you have all expected bytes |
| fds as locals, handler can't see them | make them globals |
