# Session 013 — String Parsing

**Date:** 2026-03-25
**Duration:** ~1 session
**Folder:** `013-parsing/`

---

## What We Covered

### strtok
- Splits a string by a delimiter — destructive, writes `\0` bytes into the original string in place
- First call takes the string. All subsequent calls take `NULL` — that means "continue from the bookmark"
- One global bookmark per program — only one active `strtok` session at a time
- After `strtok` finishes, the original string is destroyed — don't use it as a whole string afterwards

```c
char line[] = "alice,30,engineer";
char *token = strtok(line, ",");
while (token != NULL) {
    printf("%s\n", token);
    token = strtok(NULL, ",");
}
```

### strstr
- Finds the first occurrence of a substring and returns a pointer to it — returns NULL if not found
- Returns a pointer into the middle of the original string, not a copy
- Pointer arithmetic: `end - request` gives the byte offset, `end + 4` skips 4 characters forward

```c
char *end = strstr(request, "\r\n\r\n");
if (end != NULL) {
    char *body = end + 4;   // skip past the blank line
}
```

### sscanf
- Like `scanf` but reads from a string instead of stdin — no global state, doesn't touch strtok's bookmark
- Returns the number of items successfully parsed
- `%63[^:]` — read up to 63 chars, stop at `:`
- `%127[^\r]` — read up to 127 chars, stop at `\r`
- `[^x]` means "anything except x" — safer than `%s` which stops at any whitespace

```c
char key[64], value[128];
int parsed = sscanf(line, "%63[^:]: %127[^\r]", key, value);
if (parsed == 2) { /* valid header */ }
```

### Parsing an HTTP Request Line
```c
char method[8], path[256], version[16];
sscanf(raw, "%7s %255s %15s", method, path, version);
```
- Use `sscanf` not `strtok` — keeps strtok's bookmark free for header parsing
- Width limiters (`%7s`, `%255s`) prevent buffer overflow

### Parsing HTTP Headers
```c
char *line = strtok(raw, "\r\n");   // skip request line
line = strtok(NULL, "\r\n");        // first header
while (line != NULL) {
    int parsed = sscanf(line, "%63[^:]: %127[^\r]", key, value);
    if (parsed == 2) { /* store header */ }
    line = strtok(NULL, "\r\n");
}
```

### The Critical strtok Rule
- `strtok` has **one global bookmark per program** — not one per string
- Calling `strtok` on a second string hijacks the bookmark and loses your place in the first
- Fix: use `sscanf` for any parsing that runs alongside a `strtok` session

```
strtok  — owns one global bookmark, destructive, one active session at a time
sscanf  — reads a string without touching strtok's bookmark, safe to interleave
```

---

## The Full Parser

```c
void parse_request(char *raw, Request *req) {
    // 1. grab body pointer BEFORE strtok destroys the string
    char *body = strstr(raw, "\r\n\r\n");
    req->body = (body != NULL) ? body + 4 : NULL;

    // 2. parse request line with sscanf — doesn't touch strtok bookmark
    char method[8], path[256], version[16];
    if (sscanf(raw, "%7s %255s %15s", method, path, version) != 3) return;

    strncpy(req->method,  method,  sizeof(req->method)  - 1);
    strncpy(req->path,    path,    sizeof(req->path)    - 1);
    strncpy(req->version, version, sizeof(req->version) - 1);

    // 3. parse headers with strtok
    char *line = strtok(raw, "\r\n");    // consumes request line
    line = strtok(NULL, "\r\n");         // first header
    req->header_count = 0;
    while (line != NULL && req->header_count < MAX_HEADERS) {
        int parsed = sscanf(line, "%63[^:]: %127[^\r]",
            req->headers[req->header_count].key,
            req->headers[req->header_count].value);
        if (parsed == 2) req->header_count++;
        line = strtok(NULL, "\r\n");
    }
}
```

Key ordering: `strstr` before `strtok` — once `strtok` starts writing `\0` bytes, the `\r\n\r\n` you're looking for gets destroyed.

---

## Gotchas Hit
- `strtok` called on a second string mid-session hijacks the global bookmark — headers returned 0 until fixed
- `sizeof(pointer)` returns 8 on 64-bit, not the buffer size — always use `sizeof(destination)` in `strncpy`
- `if` instead of `while` for header loop — only parsed one header
- `strstr` must be called before `strtok` starts — strtok destroys the delimiters you're searching for

---

## Files Written
- `01-strtok.c` — split a CSV line, print each field
- `02-strstr.c` — find `\r\n\r\n`, extract body pointer
- `03-http-line.c` — parse HTTP request line into method/path/version
- `04-headers.c` — parse header block into a struct array with sscanf
- `05-full-request.c` — full request parser combining all techniques

---

## Where We Stopped
Finished `013-parsing`. All 5 exercises done.

## What's Next
- **Concurrency I — pthreads** — spawn threads, pass arguments, join
- Thread-per-connection pattern that the HTTP server will use
