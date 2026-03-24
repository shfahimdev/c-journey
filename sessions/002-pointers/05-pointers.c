#include <stdio.h>
#include <stdlib.h>

int main() {
    int *p = NULL;

    if (p == NULL) {
        printf("pointer is null, not safe to dereference\n");
    }

    p = malloc(sizeof(int));
    if (p == NULL) {
        printf("malloc failed\n");
        return 1;
    }

    *p = 7;
    printf("value = %d\n", *p);
    free(p);

    p = NULL;

    return 0;
}
