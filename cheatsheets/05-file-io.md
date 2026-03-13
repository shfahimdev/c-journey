# Cheatsheet — 05 File I/O

## Open & Close
```c
FILE *f = fopen("file.txt", "w");  // open
if (f == NULL) {
    perror("fopen");               // print error
    return 1;                      // exit with failure
}
fclose(f);                         // always close
```

## fopen Modes
| Mode | Meaning |
|------|---------|
| `"r"` | read — file must exist |
| `"w"` | write — creates or truncates |
| `"a"` | append |
| `"rb"` / `"wb"` | binary read/write |

---

## Text I/O
```c
// write
fprintf(f, "hello %s\n", name);

// read line by line
char line[64];
while (fgets(line, sizeof(line), f) != NULL) {
    printf("%s", line);   // fgets keeps \n — don't add another
}
```

---

## Binary I/O
```c
// write array
int arr[5] = {10, 20, 30, 40, 50};
fwrite(arr, sizeof(int), 5, f);

// read array
int result[5] = {0};
fread(result, sizeof(int), 5, f);
```

## fread / fwrite signature
```c
fwrite(ptr, size_of_one_item, number_of_items, file);
fread(ptr, size_of_one_item, number_of_items, file);
// both return number of items actually read/written
```

---

## Struct Serialization
```c
// write
fwrite(&player, sizeof(Player), 1, f);

// read
Player p = {0};
fread(&p, sizeof(Player), 1, f);
```

---

## Linked List Serialization
```c
// write — walk list, write each node's data
Node *current = head;
while (current != NULL) {
    fwrite(&current->data, sizeof(Player), 1, f);
    current = current->next;
}

// read — loop until fread returns 0
Player data;
while (fread(&data, sizeof(Player), 1, f) == 1) {
    append(&head, data.name, data.health);
}
```

---

## Error Handling
```c
FILE *f = fopen("ghost.txt", "r");
if (f == NULL) {
    perror("fopen");   // "fopen: No such file or directory"
    return 1;
}

size_t n = fread(buffer, 1, 128, f);
printf("read %zu bytes\n", n);   // %zu for size_t
```

---

## Gotchas
- Always check `fopen` returns non-NULL before using
- `fclose` before reopening in a different mode
- `fgets` keeps `\n` — don't double up in printf
- Uninitialized `Node *head` must be `= NULL` — garbage value breaks append
- Can't serialize linked list in one call — `next` pointers are meaningless on disk
