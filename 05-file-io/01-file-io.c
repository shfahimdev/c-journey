#include <stdio.h>

int main() {
    FILE *f = fopen("data.txt", "w");
    if (f == NULL) {
        perror("fopen"); // prints: "fopen: No such file or directory"
        return 1;
    }
    fprintf(f, "Fahim\n");
    fprintf(f, "Aurin\n");
    fprintf(f, "Aurifa\n");

    fclose(f);

    f = fopen("data.txt", "r");
    if (f == NULL) {
        perror("fopen"); // prints: "fopen: No such file or directory"
        return 1;
    }
    char line[64];
    while (fgets(line, sizeof(line), f) != NULL) {
        printf("%s", line);
    }
    fclose(f);

    return 0;
}
