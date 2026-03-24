# Contact Book Cheatsheet

## Structs
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

Node *table[TABLE_SIZE];   // global array of linked list heads
```

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

---

## Add (prepend)
```c
void add(char *name, char *phone) {
    int index = hash(name);
    Node *new = calloc(1, sizeof(Node));   // calloc = malloc + zero-init
    strcpy(new->data.name,  name);
    strcpy(new->data.phone, phone);
    new->next    = table[index];           // point to current head
    table[index] = new;                    // new node becomes head
}
```

---

## Search
```c
Node *search(char *name) {
    int index = hash(name);
    Node *current = table[index];
    while (current != NULL) {
        if (strcmp(current->data.name, name) == 0)
            return current;               // found — return pointer to node
        current = current->next;
    }
    return NULL;                          // not found
}
```
Usage:
```c
Node *result = search("fahim");
if (result != NULL)
    printf("%s - %s\n", result->data.name, result->data.phone);
```

---

## Delete
```c
void delete(char *name) {
    int index = hash(name);
    Node *prev    = NULL;
    Node *current = table[index];
    while (current != NULL) {
        if (strcmp(current->data.name, name) == 0) {
            if (prev == NULL) table[index] = current->next;  // deleting head
            else              prev->next   = current->next;  // bypass node
            free(current);
            return;
        }
        prev    = current;
        current = current->next;
    }
    // silently does nothing if name not found
}
```

---

## Print All
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

## Save to File
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

---

## Load from File
```c
void load(char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) return;        // first run — no file yet, not an error
    Contact c;
    while (fread(&c, sizeof(Contact), 1, f) == 1)
        add(c.name, c.phone);
    fclose(f);
}
```

---

## Free Table
```c
void free_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table[i];
        while (current != NULL) {
            Node *temp = current->next;   // save next BEFORE freeing
            free(current);
            current = temp;
        }
        table[i] = NULL;
    }
}
```

---

## Main Loop Pattern
```c
int main() {
    load("contacts.bin");         // load saved data on startup

    while (1) {
        printf("\n1. Add\n2. Search\n3. Delete\n4. List all\n5. Exit\n> ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: /* add */    break;
            case 2: /* search */ break;
            case 3: /* delete */ break;
            case 4: /* list */   break;
            case 5:
                save("contacts.bin");   // persist on exit
                free_table();
                return 0;
        }
    }
}
```

---

## Key Decisions
- `calloc` over `malloc` — zero-initializes the node
- Save on exit only — not after every mutation
- `load` silently skips if file doesn't exist — correct first-run behavior
- TABLE_SIZE 16 — small enough to hit collisions naturally
- Contacts stored as raw binary structs — same pattern as sessions 005 + 008
