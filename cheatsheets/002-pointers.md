# Cheatsheet — 02 Pointers

## Core Syntax
```c
int x = 42;
int *p = &x;   // p holds the address of x
*p = 100;      // write to x through p
printf("%p", p);   // print address (use %p, not %d)
printf("%d", *p);  // print value at address
```

## The Two Operators
| Operator | Name | Meaning |
|----------|------|---------|
| `&x` | address-of | give me the address of x |
| `*p` | dereference | give me the value at address p |

---

## Pass by Reference
```c
void double_it(int *n) {
    *n = *n * 2;    // modifies original
}

int main() {
    int x = 5;
    double_it(&x);  // pass address, not value
}
```

### Common Bug
```c
void reset(int *a, int *b) {
    *a = 0;   // ✅ writes to original
    b = 0;    // ❌ only wipes local copy of address
}
```

---

## Arrays and Pointers
```c
int arr[4] = {10, 20, 30, 40};
int *p = arr;       // arr IS a pointer to first element

arr[0] == *arr      // identical
arr[i] == *(arr+i)  // identical

p[1]    // = 20
*(p+2)  // = 30
```

---

## Double Pointers
```c
int x = 99;
int *p = &x;
int **pp = &p;

*pp   // → p (a pointer)
**pp  // → 99 (the value)
```
Used when a function needs to modify a pointer itself:
```c
void allocate(int **p) {
    *p = malloc(sizeof(int));
    **p = 99;
}

int *p = NULL;
allocate(&p);   // pass address of the pointer
```

---

## malloc and free
```c
int *p = malloc(sizeof(int));   // ask OS for memory
if (p == NULL) return 1;        // always check
*p = 7;
free(p);                        // hand memory back
p = NULL;                       // prevent dangling pointer
```

| Function | What it does |
|----------|-------------|
| `malloc(size)` | allocates size bytes on heap, returns address |
| `free(p)` | releases memory back to OS |

**Every malloc needs a matching free.**

---

## NULL and Safety
```c
int *p = NULL;

if (p == NULL) {
    // not safe to dereference
}

free(p);
p = NULL;   // always null after free
```

- **Dangling pointer** — pointer still holds address after free
- **Memory leak** — malloc without free
- **Crash loudly early** > corrupt silently late

---

## Mental Model
```
x   → [42]           int, lives at some address
p   → [addr of x]    pointer, holds x's address
pp  → [addr of p]    double pointer, holds p's address

*p   = follow once  → 42
**pp = follow twice → 42
```

---

## FFmpeg Pattern
```c
AVFrame *frame = av_frame_alloc();     // malloc
uint8_t *pixels = frame->data[0];      // pointer to pixel bytes
*(pixels + i)                          // pointer arithmetic
av_frame_free(&frame);                 // free + NULL (**frame = NULL)
```
