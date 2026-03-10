# Session 002 — Pointers

**Date:** 2026-03-11
**Duration:** ~1 session
**Folder:** `02-pointers/`

---

## What We Covered

### What is a Pointer?
- A pointer is a variable that stores a **memory address** instead of a value
- Every variable lives somewhere in memory — a pointer lets you hold and pass that location
- `&x` — gives you the address of x
- `*p` — follows the address p is holding, gives you the value there (dereference)

```c
int x = 42;
int *p = &x;   // p holds the address of x
*p = 100;      // modifies x through the pointer
```

### Why Pointers Exist
- Variable names only exist at compile time — the binary has no names, only addresses
- Passing large data by value copies everything (expensive). A pointer is always 8 bytes — just the address
- Functions can't modify the caller's variables without a pointer (C passes by value by default)
- Analogy: a pointer is a **cheque**. The variable is the bank account. You pass the cheque around instead of carrying the cash.

### Pass by Reference
```c
void double_it(int *n) {
    *n = *n * 2;    // modifies the original via address
}

int main() {
    int x = 5;
    double_it(&x);  // pass the address
    printf("%d\n", x);  // prints 10
}
```
- Without `*`, you'd be modifying a local copy — original untouched
- Key bug to recognize: `b = 0` vs `*b = 0` — one reassigns the pointer, one writes to memory

### Pointers and Arrays
- An array name **is** a pointer to its first element
- `arr[0]` and `*arr` are identical
- `arr[i]` and `*(arr+i)` are identical — `[]` is just cleaner pointer arithmetic

```c
int arr[4] = {10, 20, 30, 40};
int *p = arr;
p[1]    // = 20
*(p+2)  // = 30
```

### Double Pointers (`**`)
- A pointer to a pointer — two hops to reach the value
- `*pp` → lands on a pointer
- `**pp` → lands on the actual value
- Used when a function needs to modify a pointer itself (e.g. allocate memory and hand it back)

```c
int **pp = &p;
// pp → address of p → address of x → value
```

### malloc and free
- Stack memory: allocated/freed automatically when functions run/return
- Heap memory: you manage it manually with `malloc` and `free`
- `malloc(sizeof(int))` — asks OS for a chunk of memory, returns its address
- `free(p)` — hands it back to the OS
- Every `malloc` must have a matching `free` — otherwise it's a **memory leak**

```c
int *p = malloc(sizeof(int));
*p = 99;
free(p);
p = NULL;
```

### NULL and Dangling Pointers
- After `free(p)`, the address is still written in `p` — it's now a **dangling pointer**
- Setting `p = NULL` after free prevents accidental use of freed memory
- Always check `if (p == NULL)` before dereferencing
- Crash loudly early > corrupt silently late

---

## Gotchas Hit
- `%d` for addresses shows a weird negative number — use `%p` for proper hex address output
- `b = 0` inside a function only wipes the local copy of the address — original pointer unchanged
- `free` doesn't clear the address from the pointer — always set to NULL after freeing
- Array name is a pointer — `arr` and `&arr[0]` are the same thing

---

## Files Written
- `01-pointers.c` — address of, dereference, modifying via pointer
- `02-pointers.c` — pass by reference, double_it function
- `03-pointers.c` — arrays as pointers, pointer arithmetic
- `04-pointers.c` — double pointers, malloc, allocate function
- `05-pointers.c` — NULL check, malloc, free, dangling pointer

---

## FFmpeg Connection
```c
AVFrame *frame = av_frame_alloc();   // malloc under the hood
uint8_t *pixels = frame->data[0];    // pointer to raw pixel bytes
*(pixels + i)                        // pointer arithmetic to walk pixels
av_frame_free(&frame);               // free + NULL, same pattern as exercise 5
```

---

## Where We Stopped
Finished `02-pointers`. All 5 exercises done.

## What's Next
- **Structs** — grouping data together into custom types
- **Pointer to struct** and the `->` operator
- This is where FFmpeg code becomes readable (`ctx->width`, `frame->data`)
