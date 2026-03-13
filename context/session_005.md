# Session 005 — File I/O

**Date:** 2026-03-14
**Duration:** ~1 session
**Folder:** `05-file-io/`

---

## What We Covered

### `FILE *` — the file handle
- `FILE *` is a pointer to a struct the OS manages — you never look inside it, just pass it to functions
- Same pattern as `AVCodecContext *` in FFmpeg
- `fopen` returns `NULL` on failure — always check before using

### fopen modes
| Mode | Meaning |
|------|---------|
| `"r"` | read — file must exist |
| `"w"` | write — creates or truncates |
| `"a"` | append — creates or adds to end |
| `"rb"` / `"wb"` | binary read/write |

### Core functions
```c
fopen(path, mode)              // open — returns FILE* or NULL
fprintf(f, "...", ...)         // write text
fgets(buf, size, f)            // read one line — returns NULL at EOF
fclose(f)                      // always close
fwrite(ptr, size, count, f)    // write binary
fread(ptr, size, count, f)     // read binary — returns items actually read
```

### Text vs Binary
- Text mode — human readable, newlines may be translated by OS
- Binary mode — raw bytes, no translation. Use for structs and arrays

### Buffers
- A buffer is just an array used as temporary storage while moving data
- `char line[64]` in exercise 1 was a buffer — `fgets` put bytes from the file into it
- Same idea with `fread(buffer, 1, 128, f)` — reads bytes from file into the array

### Error handling
```c
FILE *f = fopen("ghost.txt", "r");
if (f == NULL) {
    perror("fopen");   // prints: "fopen: No such file or directory"
    return 1;
}
```
- `perror` reads the global `errno` set by the failed call and prints a human-readable message
- `return 1` from main = failure. `return 0` = success. Unix convention.

### fread return value
```c
size_t n = fread(buffer, 1, 128, f);
printf("read %zu bytes\n", n);
```
- Returns number of items actually read — not bytes, items
- `size_t` is an unsigned integer type for sizes. Format specifier is `%zu`
- Returns 0 at EOF — use this to drive a read loop

### Serializing a struct
```c
fwrite(&player, sizeof(Player), 1, f);   // write one struct
fread(&player, sizeof(Player), 1, f);    // read it back
```
- Pass the address of the struct with `&`
- The struct is just bytes in memory — `fwrite` writes them, `fread` puts them back

### Serializing a linked list
- Can't write the whole list in one call — nodes are scattered on the heap, `next` pointers are meaningless on disk
- Walk the list and write each node's data one at a time
- Read back in a loop until `fread` returns 0, appending each item to a new list

```c
// write
Node *current = head;
while (current != NULL) {
    fwrite(&current->data, sizeof(Player), 1, f);
    current = current->next;
}

// read
Player data;
while (fread(&data, sizeof(Player), 1, f) == 1) {
    append(&head, data.name, data.health);
}
```

---

## Gotchas Hit
- `another_head` uninitialized — must be `= NULL` or `append`'s empty-list check reads garbage and segfaults
- `fgets` keeps the `\n` in the buffer — don't add another in `printf` or you get blank lines
- Must `fclose` before reopening in a different mode — reuse the same `FILE *f`

---

## Files Written
- `01-file-io.c` — write 3 names to a text file, read them back with fgets
- `02-file-io.c` — write an int array to binary, read it back with fread
- `03-file-io.c` — error handling with perror, fread return value, size_t
- `04-file-io.c` — serialize a single struct to binary, deserialize it back
- `05-file-io.c` — serialize a linked list to binary, reconstruct it from file

---

## Where We Stopped
Finished `05-file-io`. All 5 exercises done.

## What's Next
- **Binary trees** — recursive thinking, two pointers per node (left/right)
- **Hash tables** — arrays + linked lists combined, collision handling
- **Memory management** — writing your own allocator
