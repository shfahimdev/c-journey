# Session 004 — Enums & Linked Lists

**Date:** 2026-03-13
**Duration:** ~1 session
**Folder:** `04-linked-lists/`

---

## What We Covered

### Enums
- An enum assigns names to integers — starts at 0 and increments automatically
- Always use enum names in switch cases, not raw numbers — that's the whole point
- `const char *` is the correct return type for functions returning string literals — they're read-only
- Extract status-to-string logic into its own function for reuse across the codebase

```c
typedef enum {
    ALIVE,
    DEAD,
    UNKNOWN,
} Status;

const char *status_to_string(Status s) {
    switch (s) {
        case ALIVE:   return "alive";
        case DEAD:    return "dead";
        case UNKNOWN: return "unknown";
        default:      return "invalid";
    }
}
```

### What is a Linked List?
- A chain of nodes — each node holds data and a pointer to the next node
- The list is just a pointer to the first node (called head)
- Last node's `next` is NULL — that's how you know you've reached the end
- Array = fixed size at compile time. Linked list = grows/shrinks at runtime via malloc/free

```
head → [10|next] → [20|next] → [30|NULL]
```

### Node Definition
```c
typedef struct Node {
    int value;
    struct Node *next;   // must use "struct Node" — typedef isn't finished yet
} Node;
```

### Append
- Takes `Node **head` — double pointer needed because if the list is empty, the function must modify head itself
- Walk to last node where `current->next == NULL` (not `current == NULL` — you need to land ON the last node, not past it)

```c
void append(Node **head, int value) {
    Node *new = malloc(sizeof(Node));
    new->value = value;
    new->next  = NULL;

    if (*head == NULL) {
        *head = new;
        return;
    }

    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new;
}
```

### Delete
- Three cases: empty list, delete head, delete middle/end
- Deleting head: update `*head` to point to the second node, free the old head
- Deleting middle/end: use two pointers (prev + current), bypass the target node with `prev->next = current->next`

```c
void delete(Node **head, int value) {
    if (*head == NULL) return;

    if ((*head)->value == value) {
        Node *temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }

    Node *prev = *head;
    Node *current = (*head)->next;

    while (current != NULL) {
        if (current->value == value) {
            prev->next = current->next;
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}
```

### Walking / Freeing
```c
// print
Node *current = head;
while (current != NULL) {
    printf("%d\n", current->value);
    current = current->next;
}

// free
Node *current = head;
while (current != NULL) {
    Node *temp = current->next;  // save next BEFORE freeing
    free(current);
    current = temp;
}
```

### Strings in Linked Lists
- Can't compare strings with `==` — that compares addresses, not contents
- Use `strcmp(a, b) == 0` for equality
- Use `strcpy` to copy a string into a struct field

### Structs inside Nodes
- Node can hold any struct as its data field
- Access with `current->data.name`, `current->data.status` etc.

```c
typedef struct Node {
    Player data;
    struct Node *next;
} Node;
```

---

## Gotchas Hit
- `=` instead of `==` inside if conditions — assignment, not comparison
- `current = new` instead of `current->next = new` — only changes local pointer, doesn't link anything
- `head == NULL` instead of `*head == NULL` — checks the pointer-to-pointer, which always exists
- `==` to compare strings — use `strcmp` instead
- `free(current)` before saving `current->next` — reading freed memory, causes crash
- `construct` is not a C keyword — it's `struct`

---

## Files Written
- `01-enums.c` — Status enum, Player struct, status_to_string helper
- `02-linked-list.c` — manual 3-node list, print_list, free
- `03-linked-list.c` — append function, build list dynamically
- `04-linked-list.c` — delete by value, all three cases
- `05-linked-list.c` — linked list of Player structs, append/delete/print with enums

---

## Where We Stopped
Finished `04-linked-lists`. All 5 exercises done.

## What's Next
- **File I/O** — fopen, fread, fwrite, fclose
- Write linked list data to a file, read it back
- Or: **binary trees** — the next data structure after linked lists
