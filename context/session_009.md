# Session 009 — Practice Project: Contact Book

**Date:** 2026-03-23
**Duration:** ~1 session
**Folder:** `09-contact-book/`

---

## What We Built

A command-line contact book in a single file — `contact-book.c` — combining everything
from sessions 001–008 into one working program. Name + phone storage, O(1) lookup,
persistent binary file, clean menu loop.

---

## Data Structure

Hash table of size 16. Each slot is the head of a linked list for collision handling.

```c
#define TABLE_SIZE 16

typedef struct {
    char name[32];
    char phone[16];
} Contact;

typedef struct Node {
    Contact data;
    struct Node *next;
} Node;

Node *table[TABLE_SIZE];   // global, zero-initialized
```

Kept it lean — no email field, just name and phone. Enough to exercise the full pattern
without bloating the struct.

---

## Hash Function

```c
int hash(char *key) {
    int total = 0;
    while (*key != '\0') {
        total += *key;
        key++;
    }
    return total % TABLE_SIZE;
}
```

ASCII sum mod TABLE_SIZE. Same function from session 008.

---

## Core Operations

### add — prepend to chain
```c
void add(char *name, char *phone) {
    int index = hash(name);
    Node *new = calloc(1, sizeof(Node));   // calloc — zero-initialized
    strcpy(new->data.name,  name);
    strcpy(new->data.phone, phone);
    new->next = table[index];              // prepend
    table[index] = new;
}
```
Used `calloc` instead of `malloc` — zero-initializes the node so no garbage bytes
if a field is ever read before being written.

### search — walk the chain, strcmp
```c
Node *search(char *name) {
    int index = hash(name);
    Node *current = table[index];
    while (current != NULL) {
        if (strcmp(current->data.name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}
```
Returns the node pointer so the caller can read both name and phone from it.

### delete — prev/current two-pointer walk
```c
void delete(char *name) {
    int index = hash(name);
    Node *prev = NULL;
    Node *current = table[index];
    while (current != NULL) {
        if (strcmp(current->data.name, name) == 0) {
            if (prev == NULL) table[index] = current->next;
            else              prev->next = current->next;
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}
```
Exact same pattern as session 008 delete. Silently does nothing if name not found.

### table_print — walk every chain
```c
void table_print() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table[i];
        while (current != NULL) {
            printf("%s - %s\n", current->data.name, current->data.phone);
            current = current->next;
        }
    }
}
```

---

## File Persistence

### save
```c
void save(char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) { perror("fopen"); return; }
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table[i];
        while (current != NULL) {
            fwrite(&current->data, sizeof(Contact), 1, f);
            current = current->next;
        }
    }
    fclose(f);
}
```

### load
```c
void load(char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) return;   // first run — no file yet, silently skip
    Contact c;
    while (fread(&c, sizeof(Contact), 1, f) == 1)
        add(c.name, c.phone);
    fclose(f);
}
```
`fopen` returning NULL on first run is not an error — just means no saved data yet.
`fread` loop reads one Contact at a time until EOF. Same pattern as session 005 + 008.

### free_table — walk and free, reset slots to NULL
```c
void free_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table[i];
        while (current != NULL) {
            Node *temp = current->next;   // save next before freeing
            free(current);
            current = temp;
        }
        table[i] = NULL;
    }
}
```

---

## Main Loop

```c
int main() {
    load("contacts.bin");

    int choice;
    char name[32];
    char phone[16];

    while (1) {
        printf("\n1. Add\n2. Search\n3. Delete\n4. List all\n5. Exit\n> ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: /* add */    break;
        case 2: /* search */ break;
        case 3: /* delete */ break;
        case 4: /* list */   break;
        case 5:
            save("contacts.bin");
            free_table();
            return 0;
        default:
            printf("Invalid choice.\n");
        }
    }
}
```

- Load on startup — contacts persist across runs
- Save only on exit (case 5) — not on every add/delete
- `free_table()` before exit — valgrind clean

---

## Skills This Pulled Together

| Session | What got used |
|---------|--------------|
| 001 | printf, scanf, switch, while loop |
| 002 | pointers, malloc/free, NULL checks |
| 003 | structs, strcpy, `->` operator |
| 004 | linked list append, delete, walk |
| 005 | fopen, fread, fwrite, fclose, binary mode |
| 008 | hash function, table array, chaining, free_table |

---

## Design Decisions Worth Noting
- `calloc` over `malloc` — safer default, zero-initializes fields
- Save only on exit, not after every mutation — simpler, fine for a small CLI tool
- `load` silently returns on NULL fopen — correct behavior for first run, not a bug
- No email field — kept Contact lean, name + phone is enough to test the full pattern
- TABLE_SIZE 16 — small enough to see collisions happen naturally during testing

---

## Where We Stopped
Finished `09-contact-book`. Single file, full CLI, binary persistence, valgrind clean.

## What's Next
- **Processes & Signals** — fork, exec, waitpid, signals, zombies
