# Session 003 — Structs

**Date:** 2026-03-12
**Duration:** ~1 session
**Folder:** `03-structs/`

---

## What We Covered

### What is a Struct?
- A struct groups related data into a custom type
- `typedef struct { ... } Name;` avoids writing `struct Name` everywhere
- Members accessed with `.` operator on stack variables
```c
typedef struct {
    int x;
    int y;
} Point;

Point p;
p.x = 10;
p.y = 20;
```

### Pass by Value vs Pointer
- Passing a struct by value copies the whole thing — modifications don't affect the original
- Passing a pointer lets you modify the original, and is cheaper (always 8 bytes regardless of struct size)

### `->` Operator
- Used when you have a pointer to a struct
- `p->x` is identical to `(*p).x`
- Rule: stack variable → `.` / pointer → `->`
```c
void modify(Point *p) {
    p->x = 999;   // modifies original
}
modify(&p);
```

### Structs on the Heap
- `malloc(sizeof(Point))` allocates the struct on the heap
- Always returns a pointer — so always use `->` from that point on
- Same malloc/free/NULL rules as before
```c
Point *p = malloc(sizeof(Point));
p->x = 10;
free(p);
p = NULL;
```

### Strings in Structs
- `char name[32]` inside a struct can't be assigned with `=`
- Arrays don't copy with `=` — use `strcpy` from `string.h`
- Manual char-by-char loop and `strcpy` do the exact same thing under the hood
```c
strcpy(p->name, name);
```

### Array of Structs
- Structs can be stored in arrays like any other type
- Can be initialized inline with `{}` syntax
- Access members with `party[i].name` — dot because `party[i]` is a value, not a pointer
```c
Player party[3] = {
    {"fahim",  100},
    {"gandalf", 80},
    {"frodo",   60},
};

for (int i = 0; i < 3; i++) {
    printf("%s — hp: %d\n", party[i].name, party[i].health);
}
```

---

## Gotchas Hit
- Missing `;` after array-of-structs initializer `}` — gcc reports error on the *next* line which is confusing
- `=` doesn't copy strings — use `strcpy`
- Stack variable → `.`, pointer → `->` — mixing them up is a common early mistake

---

## Files Written
- `01-structs.c` — define Point, access with `.`, pass by value
- `02-structs.c` — pass by value vs pass by pointer, `->` operator
- `03-structs.c` — malloc a struct, use `->`, free it
- `04-structs.c` — Player struct, create/print/damage functions, strcpy
- `05-structs.c` — array of structs, inline initialization, loop over members

---

## Where We Stopped
Finished `03-structs`. All 5 exercises done.

## What's Next
- **Enums** — named integer constants, often paired with structs
- **Linked lists** — structs that point to other structs (first real data structure)
- Or jump straight to **file I/O** if you want something practical
