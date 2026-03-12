# Cheatsheet — 03 Structs

## Define a Struct
```c
typedef struct {
    int x;
    int y;
} Point;
```

## Stack vs Heap
```c
// stack — use .
Point p;
p.x = 10;

// heap — use ->
Point *p = malloc(sizeof(Point));
p->x = 10;
free(p);
p = NULL;
```

## . vs ->
| Situation | Operator | Example |
|-----------|----------|---------|
| Stack variable | `.` | `p.x` |
| Pointer to struct | `->` | `p->x` |

`p->x` is identical to `(*p).x`

## Pass by Pointer
```c
void modify(Point *p) {
    p->x = 999;   // modifies original
}
modify(&p);
```

## Strings in Structs
```c
#include <string.h>

typedef struct {
    char name[32];
} Player;

strcpy(p->name, "fahim");   // can't use = for strings
```

## Array of Structs
```c
Player party[3] = {
    {"fahim",  100},
    {"gandalf", 80},
};

for (int i = 0; i < 3; i++) {
    printf("%s\n", party[i].name);
}
```

## FFmpeg Connection
```c
AVCodecContext *ctx = avcodec_alloc_context3(codec);  // malloc a struct
ctx->width  = 1920;   // -> because it's a pointer
ctx->height = 1080;
avcodec_free_context(&ctx);   // free + NULL
```
