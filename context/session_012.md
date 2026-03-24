# Session 012 — Sockets II (Client/Server)

**Date:** 2026-03-25
**Duration:** ~1 session
**Folder:** `012-sockets-2/`

---

## What We Covered

### SO_REUSEADDR
- Already introduced in session 011 — this session locked it in as a habit
- Without it, restarting a server after a crash blocks on "address already in use" because the OS holds the port in TIME_WAIT
- Add it after every `socket()` call, no exceptions

```c
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```

### Partial Reads
- `read()` is not guaranteed to return all the bytes you asked for in one call
- On a real network, data arrives in packets — a single message can come in multiple chunks
- The fix is a loop that accumulates into a buffer until the expected total is reached

```c
char buffer[MSG_SIZE];
int total = 0;
while (total < MSG_SIZE) {
    int n = read(client_fd, buffer + total, MSG_SIZE - total);
    if (n <= 0) break;   // 0 = disconnected, -1 = error
    total += n;
}
```

- `buffer + total` — write into the next available slot, not the start
- `MSG_SIZE - total` — only ask for what's still missing
- This is exactly the loop your HTTP server will use to collect headers

### SIGPIPE
- If a client disconnects while you're still writing to it, the kernel sends `SIGPIPE` to your process
- Default action is to kill the process — silently, with no error message
- Fix: ignore it with `SIG_IGN` and handle write errors through return values instead

```c
signal(SIGPIPE, SIG_IGN);
```

### Graceful Shutdown
- On `SIGINT` (Ctrl+C), close all open file descriptors before exiting
- `server_fd` and `client_fd` must be globals so the signal handler can reach them
- Use `_exit(0)` inside signal handlers — `exit()` is not signal-safe, and `return` doesn't exit the process

```c
int server_fd;
int client_fd;

void sigint_handler(int sig) {
    close(server_fd);
    close(client_fd);
    _exit(0);
}

signal(SIGINT, sigint_handler);
```

---

## Gotchas
- `return` inside a signal handler just returns to wherever the signal interrupted — use `_exit(0)` to actually exit
- `SIGPIPE` should be ignored with `SIG_IGN`, not handled with a custom function — you don't want to exit on a broken pipe, you want to keep running and catch the error from `write()`'s return value
- Partial reads are not an edge case — on localhost they rarely show up, but on a real network they will. Always loop.

---

## Files Written
- `05-reuseaddr.c` — server with SO_REUSEADDR, crash and restart without waiting
- `06-partial.c` — server with partial read loop, tested with python3 sending 32+32 bytes with a 1s delay
- `07-shutdown.c` — clean shutdown on SIGINT, globals for fds, SIGPIPE ignored

---

## Observed in Terminal
- `06-partial.c` with curl: got all 64 bytes in one read (localhost is fast)
- `06-partial.c` with python3 chunked sender: two separate `got 32 bytes` lines — partial read confirmed
- `07-shutdown.c`: Ctrl+C exited cleanly with no error output

---

## Why This Matters for the HTTP Server
- `SO_REUSEADDR` saves 20 minutes of debugging per session — port never gets stuck
- The partial read loop is not optional — HTTP headers routinely arrive in multiple packets in production
- `SIGPIPE` ignored means a client closing the connection mid-response won't kill the server
- Graceful shutdown means you can restart the server cleanly during development without stale state

---

## Where We Stopped
Finished `012-sockets-2`. All 3 exercises done.

## What's Next
- **String Parsing** — `strtok`, `strstr`, `sscanf`, parsing HTTP request lines and headers
