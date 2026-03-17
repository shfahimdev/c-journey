#include <stdio.h>

typedef struct {
    int x;
    int y;
} Point;

void printPoint(Point p) { printf("Point : (%d, %d)\n", p.x, p.y); }

int main() {
    Point p;
    p.x = 10;
    p.y = 20;

    printPoint(p);

    p.x = 69;
    printPoint(p);

    return 0;
}
