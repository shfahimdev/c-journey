#include <stdio.h>
#include <string.h>

int main() {
    char line[] = "fahim,18,marlboro";

    char *token = strtok(line, ",");
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, ",");
    }

    return 0;
}
