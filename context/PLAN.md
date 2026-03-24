# HTTP Server — Master Plan

**Goal:** Build a production-like HTTP server in C from scratch.
**Started:** 2026-03-20
**Folder:** `~/Desktop/c-journey/`

---

## How This Works
- Each session: follow the plan below, build the exercises, write `context/session_00N.md`
- After each session: delete that session block from this file
- If a session runs long: split it, update the plan accordingly

---

## Progress

| # | Topic | Status |
|---|-------|--------|
| 001 | C Basics | ✅ done |
| 002 | Pointers | ✅ done |
| 003 | Structs | ✅ done |
| 004 | Enums & Linked Lists | ✅ done |
| 005 | File I/O | ✅ done |
| 006 | Binary Trees | ✅ done |
| 007 | BST Deletion | ✅ done |
| 008 | Hash Tables | ✅ done |
| 009 | Practice — Contact Book | ✅ done |
| 010 | Processes & Signals | ✅ done |
| 011 | Sockets I — TCP basics |✅ done |
| 012 | Sockets II — client/server | ⬜ planned |
| 013 | String Parsing | ⬜ planned |
| 014 | Concurrency I — pthreads | ⬜ planned |
| 015 | Concurrency II — mutexes & race conditions | ⬜ planned |
| 016 | HTTP Server — Phase 1 (accept & print) | ⬜ planned |
| 017 | HTTP Server — Phase 2 (hardcoded response) | ⬜ planned |
| 018 | HTTP Server — Phase 3 (serve files from disk) | ⬜ planned |
| 019 | HTTP Server — Phase 4 (multithreaded) | ⬜ planned |
| 020 | HTTP Server — Phase 5 (errors, headers, MIME) | ⬜ planned |
| 021 | HTTP Server — Phase 6 (stress test & hardening) | ⬜ planned |

---

## Session 012 — Sockets II (Client/Server)
**Folder:** `012-sockets-2/`

### What to cover
- `SO_REUSEADDR` — why your port gets stuck after a crash and how to fix it
- `htons` / `ntohs` — byte order (network is big-endian, x86 is little-endian)
- `inet_pton` / `inet_ntop` — convert IP strings to binary and back
- Handling partial reads — `read()` may not return everything at once
- Handling `SIGPIPE` — writing to a closed socket crashes your server without this
- Graceful shutdown — drain, close, cleanup

### Key concepts
```c
// fix "address already in use" after crash
int opt = 1;
setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

// ignore SIGPIPE — handle errors with return values instead
signal(SIGPIPE, SIG_IGN);

// partial read loop
int total = 0;
while (total < expected) {
    int n = read(fd, buffer + total, expected - total);
    if (n <= 0) break;
    total += n;
}
```

### Exercises
- `05-reuseaddr.c` — add SO_REUSEADDR, crash and restart without waiting
- `06-partial.c` — server that handles a client sending data in chunks
- `07-shutdown.c` — clean shutdown on SIGINT, close all fds, free memory

### Why it matters for the HTTP server
`SO_REUSEADDR` alone will save you 20 minutes of debugging per session. Partial reads are a real production bug — HTTP headers can arrive in multiple packets. SIGPIPE will kill your server silently if you don't handle it.

---

## Session 013 — String Parsing
**Folder:** `013-parsing/`

### What to cover
- `strtok` — split a string by delimiter (destructive)
- `strstr` — find a substring
- `sscanf` — parse formatted data from a string
- `memcmp` — compare raw bytes
- `strchr` — find first occurrence of a character
- Manual byte-by-byte parsing — walking a buffer with a pointer
- Parsing HTTP request line: `GET /path HTTP/1.1`
- Parsing headers: `Key: Value\r\n`
- Finding the end of headers: `\r\n\r\n`

### Key concepts
```c
// split request line
char *method  = strtok(line, " ");   // "GET"
char *path    = strtok(NULL, " ");   // "/index.html"
char *version = strtok(NULL, "\r"); // "HTTP/1.1"

// find end of headers
char *body = strstr(buffer, "\r\n\r\n");
if (body) body += 4;   // skip past the blank line

// parse a header
char key[64], value[128];
sscanf(header_line, "%63[^:]: %127[^\r]", key, value);
```

### Exercises
- `01-strtok.c` — split a CSV line, print each field
- `02-strstr.c` — find a substring, extract everything after it
- `03-http-line.c` — parse a raw HTTP request line into method/path/version
- `04-headers.c` — parse a full HTTP header block into a struct array
- `05-full-request.c` — parse a complete raw HTTP request (method + headers + body)

### Why it matters for the HTTP server
The entire server logic is driven by parsing. You read bytes from the socket, then you need to extract method, path, headers. A bug here means serving the wrong file, wrong status code, or crashing on malformed input.

---

## Session 014 — Concurrency I (pthreads)
**Folder:** `014-threads-1/`

### What to cover
- What a thread is — shares memory with parent, unlike fork
- `pthread_create` — spawn a thread
- `pthread_join` — wait for a thread to finish
- `pthread_exit` — terminate a thread
- Passing arguments to threads — must heap-allocate, never pass stack address
- Thread-per-connection pattern — spawn a thread for each `accept()`

### Key concepts
```c
#include <pthread.h>   // link with -lpthread

void *handle(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);
    // read, respond, close
    return NULL;
}

// in main loop
int *fd_ptr = malloc(sizeof(int));
*fd_ptr = client_fd;
pthread_t tid;
pthread_create(&tid, NULL, handle, fd_ptr);
pthread_detach(tid);   // don't need to join — auto-cleanup on exit
```

### Exercises
- `01-thread.c` — spawn 3 threads, each prints its ID
- `02-args.c` — pass an int to each thread via heap pointer
- `03-join.c` — spawn threads, join them all, collect return values
- `04-server.c` — echo server that spawns a thread per connection

### Why it matters for the HTTP server
This is what makes your server handle more than one client. Without threads (or fork), every other client blocks while you serve one. Thread-per-connection is the simplest model and the right starting point.

---

## Session 015 — Concurrency II (Mutexes & Race Conditions)
**Folder:** `015-threads-2/`

### What to cover
- Race condition — two threads read/modify the same variable, result is unpredictable
- `pthread_mutex_t` — a lock. Only one thread holds it at a time
- `pthread_mutex_lock` / `pthread_mutex_unlock`
- Deadlock — two threads each waiting for a lock the other holds
- What shared state your server actually has (connection counter, log file, config)
- When NOT to use a mutex — stateless request handlers need no locking

### Key concepts
```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

void *increment(void *arg) {
    pthread_mutex_lock(&lock);
    counter++;
    pthread_mutex_unlock(&lock);
    return NULL;
}
```

### Exercises
- `05-race.c` — two threads increment a counter 100k times without a lock — show the wrong result
- `06-mutex.c` — same, with a mutex — show the correct result
- `07-deadlock.c` — demonstrate a deadlock, then fix it
- `08-log.c` — threaded server with a shared log file, mutex-protected writes

### Why it matters for the HTTP server
Your server will have at least one piece of shared state — probably a request counter or a log. One unsynchronized write from two threads corrupts the file or crashes. You need to know exactly where to put locks and why.

---

## Session 016 — HTTP Server Phase 1 (Accept & Print)
**Folder:** `016-http-server-1/`

### What to build
A server that:
- Listens on port 8080
- Accepts a connection
- Reads whatever the client sent
- Prints it to the terminal (raw bytes)
- Closes the connection

### Test with
```bash
curl http://localhost:8080
telnet localhost 8080
```

### Why this phase
Before you respond to anything, you need to see what's actually arriving. This phase is purely about getting the socket loop right and observing raw HTTP requests. No parsing yet.

---

## Session 017 — HTTP Server Phase 2 (Hardcoded Response)
**Folder:** `017-http-server-2/`

### What to build
A server that:
- Accepts a connection
- Reads the request (doesn't parse yet — just consume it)
- Sends a hardcoded valid HTTP response
- Browser should render "Hello, World"

### The response to send
```
HTTP/1.1 200 OK\r\n
Content-Type: text/html\r\n
Content-Length: 13\r\n
\r\n
Hello, World!
```

### Test with
```bash
curl -v http://localhost:8080
# open http://localhost:8080 in browser
```

---

## Session 018 — HTTP Server Phase 3 (Serve Files from Disk)
**Folder:** `018-http-server-3/`

### What to build
A server that:
- Parses the request path from the HTTP request line
- Maps it to a file on disk (e.g. `/index.html` → `./public/index.html`)
- Reads the file with `fopen`/`fread`
- Sends it with correct `Content-Length`
- Returns 404 if the file doesn't exist

### Key challenges
- Path traversal attack — `GET /../../../etc/passwd` — must sanitize the path
- Binary files (images) — can't use text mode, must use `"rb"`
- Large files — can't load the whole thing into one buffer, must send in chunks

---

## Session 019 — HTTP Server Phase 4 (Multithreaded)
**Folder:** `019-http-server-4/`

### What to build
- Spawn a thread per `accept()`
- Each thread handles one request independently
- Main thread keeps looping on `accept()`
- Use `pthread_detach` so threads clean up themselves

### Test with
```bash
ab -n 500 -c 20 http://localhost:8080/index.html
```

---

## Session 020 — HTTP Server Phase 5 (Errors, Headers, MIME)
**Folder:** `020-http-server-5/`

### What to build
- Proper status codes: 200, 404, 400, 500
- MIME type detection by file extension
- Custom error pages (404.html, 500.html)
- `Server` header, `Date` header

---

## Session 021 — HTTP Server Phase 6 (Stress Test & Hardening)
**Folder:** `021-http-server-6/`

### What to do
- Hammer with `ab` at high concurrency — find where it breaks
- Run under valgrind — find any leaks under load
- Test malformed requests
- Add request size limits
- Add basic logging — timestamp, method, path, status code, response time
- Measure: requests/second, latency p50/p99

### Stretch goals (optional)
- `epoll` — event-driven I/O
- Keep-alive — reuse the connection for multiple requests
- TLS via OpenSSL — serve over HTTPS

---

## Notes
- Compiler flags to use throughout: `gcc -Wall -Wextra -g -fsanitize=address`
- Always run valgrind on final exercise of each session
- Test tools: `curl -v`, `telnet`, `ab`, browser devtools network tab
- Reference: `man 2 socket`, `man 2 accept`, `man 7 tcp`
