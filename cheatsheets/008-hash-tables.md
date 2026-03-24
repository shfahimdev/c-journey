# Cheatsheet — 08 Hash Tables

## Structure
```c
#define TABLE_SIZE 8

typedef struct Node {
    char key[32];
    int value;
    struct Node *next;
} Node;

Node *table[TABLE_SIZE];   // global — zero-initialized automatically
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
- Sums ASCII values, mods by table size
- Anagrams collide: "abc", "bca", "cab" → same index

---

## Insert (prepend to chain)
```c
void insert(char *key, int value) {
    int index = hash(key);
    Node *new = malloc(sizeof(Node));
    strcpy(new->key, key);
    new->value = value;
    new->next = table[index];   // prepend
    table[index] = new;
}
```

---

## Lookup
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
- Returns node pointer or NULL
- Must use `strcmp` — never `==` on strings

---

## Delete
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
- Same three-case pattern as linked list delete

---

## Free Entire Table
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

---

## Binary Serialization
```c
// save
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

// load
void load_table(char *filename) {
    FILE *f = fopen(filename, "rb");
    char key[32];
    int value;
    while (fread(key, sizeof(key), 1, f) == 1 &&
           fread(&value, sizeof(value), 1, f) == 1) {
        insert(key, value);
    }
    fclose(f);
}
```

---

## malloc vs calloc
| Function | Zeros memory? | Use when |
|----------|--------------|----------|
| `malloc(size)` | No | you'll write all fields immediately |
| `calloc(1, size)` | Yes | safer default, fields may be read before written |

---

## Key Ideas
- Hash table = array of linked lists
- Collision = two keys hash to the same index → they chain at that slot
- Lookup always uses `strcmp` to find the right key within the chain
- Prepend is O(1) — no need to walk to the end on insert
- `next` pointers are meaningless on disk — serialize key+value only, re-insert on load

---

## Gotchas
- `==` to compare keys — always `strcmp`
- Forgetting `table[i] = NULL` in free_table — leaves dangling pointers
- Saving `next` pointer to file — it's a memory address, meaningless after reload
- `calloc` vs `malloc` — either works if you write all fields, but `calloc` is safer
