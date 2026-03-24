#include <stdio.h>

typedef struct {
    char name[32];
    int age;
} Person;

int main() {
    Person fahim = {"Fahim", 18};

    FILE *f = fopen("person.bin", "wb");
    fwrite(&fahim, sizeof(Person), 1, f);
    fclose(f);

    Person random;
    f = fopen("person.bin", "rb");
    fread(&random, sizeof(Person), 1, f);
    fclose(f);

    printf("%s - %d\n", random.name, random.age);

    return 0;
}
