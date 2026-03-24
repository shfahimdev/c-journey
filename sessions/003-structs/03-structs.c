#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Point;

int main() {
    Point *p = malloc(sizeof(Point));
    if (p == NULL)
        return 1;

    p->x = 10;
    p->y = 20;
    printf("Point : (%d, %d)\n", p->x, p->y);

    free(p);
    p = NULL;
}
