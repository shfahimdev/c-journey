# Session 008 — Hash Tables

**Date:** 2026-03-17
**Duration:** ~1 session
**Folder:** `08-hash-tables/`

---

## What We Covered

### What is a Hash Table?
- A hash table is an array of linked lists — combines O(1) array indexing with linked list collision handling
- A hash function maps a key (string) to an index in the array
- Multiple keys can hash to the same index — this is called a **collision**
- Collisions are handled by chaining: each slot in the array is the head of a linked list

```
table[0] → NULL
table[1] → [ali:30] → NULL
table[2] → [nadia:35] → [aurin:29] → NULL
...
```

### Node Definition
```c
typedef struct Node {
    char key[32];
    int value;
    struct Node *next;
} Node;

Node *table[TABLE_SIZE];   // global array of pointers — zero-initialized
```

### Hash Function
- Sums the ASCII values of each character in the key, mods by table size
- Simple but has collisions — keys that are anagrams of each other ("abc", "bca", "cab") hash identically

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

### Insert
- Hash the key to get the index
- Prepend the new node to the front of the chain at that index (prepend, not append — simpler and O(1))
- `new->next = table[index]; table[index] = new;` is the prepend pattern

```c
void insert(char *key, int value) {
    int index = hash(key);
    Node *new = malloc(sizeof(Node));
    strcpy(new->key, key);
    new->value = value;
    new->next = table[index];
    table[index] = new;
}
```

### Lookup
- Hash the key, walk the chain at that index, compare keys with `strcmp`
- Returns a pointer to the node if found, NULL if not

```c
Node *lookup(char *key) {
    int index = hash(key);
    Node *current = table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}
```

### Delete
- Same three-case linked list deletion pattern from session 004
- `prev` + `current` two-pointer walk, bypass the node, free it

```c
void delete(char *key) {
    int index = hash(key);
    Node *prev = NULL;
    Node *current = table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL)
                table[index] = current->next;
            else
                prev->next = current->next;
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}
```

### free_table
- Walk every chain in the array and free every node
- Set each slot back to NULL after freeing its chain

```c
void free_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table[i];
        while (current != NULL) {
            Node *temp = current->next;
            free(current);
            current = temp;
        }
        table[i] = NULL;
    }
}
```

### Serialization (save/load)
- Walk every chain, write each node's key and value to a binary file
- On load, read key+value pairs in a loop until fread returns 0, re-insert each one
- Same pattern as linked list serialization from session 005 — just with an outer loop over the array

```c
void save_table(char *filename) {
    FILE *f = fopen(filename, "wb");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table[i];
        while (current != NULL) {
            fwrite(current->key, sizeof(current->key), 1, f);
            fwrite(&current->value, sizeof(current->value), 1, f);
            current = current->next;
        }
    }
    fclose(f);
}

void load_table(char *filename) {
    FILE *f = fopen(filename, "rb");
    char key[32];
    int value;
    while (fread(key, sizeof(key), 1, f) == 1 && fread(&value, sizeof(value), 1, f) == 1) {
        insert(key, value);
    }
    fclose(f);
}
```

### calloc vs malloc
- `calloc(1, sizeof(Node))` — allocates and **zero-initializes** all bytes
- `malloc(sizeof(Node))` — allocates without zeroing (garbage bytes until you write them)
- Either works here because you immediately `strcpy` the key and set the value
- `calloc` is the safer habit when fields might be read before being explicitly written

---

## Collision Handling
- Collision = two different keys hash to the same index
- Chaining resolves this: the slot holds a linked list, all colliding keys live in the same chain
- Example: "abc", "bca", "cab" all have the same ASCII sum → same index → all chained at that slot
- Lookup still works correctly because it compares keys with `strcmp`, not just the index

---

## Files Written
- `01-hash-table.c` — Node struct, hash function, insert, print_table
- `02-hash-table.c` — added lookup, missing key returns NULL
- `03-hash-table.c` — added delete with prev/current two-pointer pattern
- `04-hash-table.c` — collision stress test with "abc"/"bca"/"cab", free_table
- `05-hash-table.c` — save_table and load_table (binary file I/O), calloc

---

## Where We Stopped
Finished `08-hash-tables`. All 5 exercises done.

## What's Next
- **Dynamic resizing** — rehash the table when load factor gets too high
- **Better hash functions** — djb2 or FNV to reduce collisions
- **Practice project** — contact book using a hash table for O(1) lookup
