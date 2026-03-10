# Session 001 — C Basics

**Date:** 2026-03-10
**Duration:** ~1 session
**Folder:** `01-basics/`

---

## What We Covered

### Environment Setup
- Compiler: `gcc`
- Editor: Neovim with NvChad (already set up)
- LSP: `clangd` via `clang` package
- Build tool: `Makefile` with auto-rule for any `.c` file
- Installed: `gcc`, `gdb`, `valgrind`, `make`, `nasm`, `bear`

### Makefile Pattern
```makefile
CC = gcc
CFLAGS = -Wall -Wextra

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f <filename>
```
- `$@` = output name
- `$<` = input file
- Run with: `make <filename> && ./<filename>`
- Copy to each new folder: `cp Makefile ../02-pointers/`

---

## Key Concepts Learned

### Types
| Type | Format | Example |
|------|--------|---------|
| `int` | `%d` | `int a = 5;` |
| `float` | `%f` / `%.2f` | `float b = 3.14;` |
| `char[]` | `%s` | `char name[] = "fahim";` |

### Integer vs Float Division
```c
int a = 7 / 2;      // = 3, remainder thrown away
float b = 7.0 / 2;  // = 3.5, proper division
```
C doesn't figure out types for you — you declare everything explicitly.

### Functions
```c
int add(int a, int b) {    // returns int, takes two ints
    return a + b;
}

void greet(char a[]) {     // returns nothing
    printf("%s\n", a);
}
```
- Must declare return type and argument types
- `void` = returns nothing
- `void` is a keyword — can't use it as a function name

### Loops
```c
for (int i = 0; i < 5; i++) { }

while (x > 0) {
    x -= 3;   // same as x = x - 3
}
```

### printf Format Specifiers
| Specifier | Type |
|-----------|------|
| `%d` | int |
| `%f` | float |
| `%.2f` | float, 2 decimal places |
| `%s` | string (char[]) |
| `%p` | pointer/address |
| `%x` | hexadecimal |

### Headers
- `#include <stdio.h>` — standard input/output (printf, scanf, etc.)
- Header files are "menus" — they tell your program what functions exist
- Without them, gcc guesses and things break silently

---

## Gotchas Hit
- `void` is a reserved keyword — can't name a function `void`
- `int divide(int a, int b)` loses decimals — use `float` if you need precision
- Makefile indentation must be **TAB**, not spaces
- `$<` in Makefile got corrupted by terminal — had to use `printf` to rewrite it
- Double `\n` if you put `\n` in both the string and the format

---

## Files Written
- `first.c` — basic arithmetic
- `variables.c` — int vs float division
- `types.c` — char[], int, float with printf
- `functions.c` — defining and calling functions
- `loops.c` — for and while loops
- `calculator.c` — all operations including power function

---

## Calculator (Final Program)
```c
#include <stdio.h>

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
float divide(float a, float b) { return a / b; }
int power(int a, int b) {
    int result = 1;
    for (int i = 1; i <= b; i++) {
        result = result * a;
    }
    return result;
}

int main() {
    int a = 5, b = 3;
    printf("add      : %d\n", add(a, b));
    printf("subtract : %d\n", subtract(a, b));
    printf("multiply : %d\n", multiply(a, b));
    printf("divide   : %.2f\n", divide(a, b));
    printf("power    : %d\n", power(a, b));
    return 0;
}
```

---

## Where We Stopped
Finished `01-basics`. Ready to start `02-pointers`.

## What's Next
- **Pointers** — `&` (address of), `*` (dereference)
- How variables live in memory
- Why FFmpeg uses pointers everywhere (AVFrame*, AVPacket*)
