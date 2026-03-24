#include <stdio.h>

typedef struct {
    int x;
    int y;
} Point;

void try_modify_value(Point p) { p.x = 99; }

void modify_pointer(Point *p) { p->x = 11; }

int main() {
    Point p;
    p.x = 10;
    p.y = 20;

    try_modify_value(p);
    printf("Point : (%d, %d) \n", p.x, p.y);

    modify_pointer(&p);
    printf("Point : (%d, %d) \n", p.x, p.y);

    return 0;
}
