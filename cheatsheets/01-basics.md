# Cheatsheet — 01 Basics

## Compile & Run
```bash
make <filename> && ./<filename>   # normal
make clean                         # remove binaries
bear -- make <filename>            # generate compile_commands.json for clangd
```

## GCC Flags
| Flag | Meaning |
|------|---------|
| `-o name` | output binary name |
| `-Wall` | enable common warnings |
| `-Wextra` | enable extra warnings |
| `-g` | include debug info (for gdb) |

---

## Types & Format Specifiers
| Type | Format | Example |
|------|--------|---------|
| `int` | `%d` | `int a = 5;` |
| `float` | `%f` / `%.2f` | `float b = 3.14;` |
| `double` | `%lf` | `double c = 3.14159;` |
| `char` | `%c` | `char x = 'a';` |
| `char[]` | `%s` | `char name[] = "fahim";` |
| hex | `%x` | `printf("%x", 255);` → `ff` |

---

## Integer vs Float Division
```c
int a   = 7 / 2;    // = 3   (truncates)
float b = 7.0 / 2;  // = 3.5 (proper)
```

---

## Functions
```c
// returns int, takes two ints
int add(int a, int b) {
    return a + b;
}

// returns nothing
void greet(char name[]) {
    printf("hello %s\n", name);
}
```

---

## Operators
```c
+   -   *   /   %       // arithmetic
==  !=  >   <  >= <=    // comparison
&&  ||  !               // logical
a += 1  a -= 1  a *= 2  // shorthand
a++  a--                // increment/decrement
```

---

## Control Flow
```c
// if/else
if (a > b) {
} else if (a == b) {
} else {
}

// for loop
for (int i = 0; i < 10; i++) { }

// while loop
while (x > 0) {
    x -= 1;
}

// switch
switch (a) {
    case 1: printf("one\n"); break;
    case 2: printf("two\n"); break;
    default: printf("other\n");
}
```

---

## Common Headers
| Header | What it gives you |
|--------|------------------|
| `stdio.h` | printf, scanf, fopen, fread |
| `stdlib.h` | malloc, free, exit |
| `string.h` | strlen, strcpy, strcmp |
| `math.h` | sqrt, pow, floor (link with `-lm`) |

---

## Makefile Template
```makefile
CC = gcc
CFLAGS = -Wall -Wextra

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f <filename>
```
