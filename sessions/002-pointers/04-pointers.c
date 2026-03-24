#include <stdio.h>
#include <stdlib.h>

void allocate(int **p) {
    *p = malloc(sizeof(int));
    **p = 99;
}

int main() {
    int *p = NULL;
    allocate(&p);
    printf("value: %d\n", *p);
    free(p);

    return 0;
}
