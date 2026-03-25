# Cheatsheet — 13 String Parsing

## strtok — split by delimiter
```c
char line[] = "alice,30,engineer";

char *token = strtok(line, ",");    // first call — pass the string
while (token != NULL) {
    printf("%s\n", token);
    token = strtok(NULL, ",");      // subsequent calls — pass NULL
}
```
- Destructive — writes `\0` into the original string
- One global bookmark — only one active session at a time
- After strtok, the original string is broken up — don't reuse it whole

---

## strstr — find a substring
```c
char *end = strstr(buffer, "\r\n\r\n");
if (end == NULL) { /* not found */ }

long offset = end - buffer;   // byte distance from start
char *body  = end + 4;        // pointer past the \r\n\r\n
```
- Returns a pointer into the original string, not a copy
- Returns NULL if not found
- Call this BEFORE strtok — strtok will destroy the delimiter you're looking for

---

## sscanf — parse formatted data from a string
```c
char key[64], value[128];
int n = sscanf(line, "%63[^:]: %127[^\r]", key, value);
if (n == 2) { /* valid parse */ }
```
- Like scanf but reads from a string — no global state
- Returns number of items successfully matched
- Safe to use alongside strtok — they don't share state

### Format string breakdown
| Pattern | Meaning |
|---------|---------|
| `%7s` | read word, max 7 chars, stop at whitespace |
| `%63[^:]` | read max 63 chars, stop at `:` |
| `%127[^\r]` | read max 127 chars, stop at `\r` |
| `[^x]` | match anything except `x` |

---

## Parse HTTP Request Line
```c
char method[8], path[256], version[16];
int n = sscanf(raw, "%7s %255s %15s", method, path, version);
if (n != 3) { /* malformed */ }
```
- Use sscanf, not strtok — keeps strtok's bookmark free for headers
- Width limiters prevent buffer overflow

---

## Parse HTTP Headers
```c
char *line = strtok(raw, "\r\n");   // consumes the request line
line = strtok(NULL, "\r\n");        // now at first header

while (line != NULL && count < MAX_HEADERS) {
    int n = sscanf(line, "%63[^:]: %127[^\r]", key, value);
    if (n == 2) {
        // store key/value
        count++;
    }
    line = strtok(NULL, "\r\n");
}
```

---

## Full Request Parser Pattern
```c
void parse_request(char *raw, Request *req) {
    // 1. body pointer FIRST — before strtok destroys \r\n\r\n
    char *body = strstr(raw, "\r\n\r\n");
    req->body = body ? body + 4 : NULL;

    // 2. request line via sscanf — doesn't touch strtok bookmark
    char method[8], path[256], version[16];
    if (sscanf(raw, "%7s %255s %15s", method, path, version) != 3) return;
    strncpy(req->method,  method,  sizeof(req->method)  - 1);
    strncpy(req->path,    path,    sizeof(req->path)    - 1);
    strncpy(req->version, version, sizeof(req->version) - 1);

    // 3. headers via strtok
    char *line = strtok(raw, "\r\n");   // skip request line
    line = strtok(NULL, "\r\n");        // first header
    req->header_count = 0;
    while (line != NULL && req->header_count < MAX_HEADERS) {
        int n = sscanf(line, "%63[^:]: %127[^\r]",
            req->headers[req->header_count].key,
            req->headers[req->header_count].value);
        if (n == 2) req->header_count++;
        line = strtok(NULL, "\r\n");
    }
}
```

---

## The Golden Rule
```
strtok  owns one global bookmark — one active session at a time
sscanf  has no global state — safe to interleave with strtok
```
Never call strtok on a second string while mid-session on the first.
Use sscanf for anything that needs to run alongside strtok.

---

## strncpy vs strcpy
```c
strncpy(dst, src, sizeof(dst) - 1);   // safe — won't overflow buffer
strcpy(dst, src);                      // unsafe — no length limit
```
Always use `sizeof(destination) - 1`, never `sizeof(pointer)`.

---

## Gotchas
- `strtok(second_string, delim)` mid-session — hijacks bookmark, loses place in first string
- `sizeof(ptr)` = 8 on 64-bit — always use `sizeof(destination buffer)` in strncpy
- `if` instead of `while` for header loop — only parses one header
- `strstr` after `strtok` starts — delimiter already destroyed, returns NULL
