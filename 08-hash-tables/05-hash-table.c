#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 8

typedef struct Node {
    char key[32];
    int value;
    struct Node *next;
} Node;

Node *table[TABLE_SIZE];

int hash(char *key) {
    int total = 0;
    while (*key != '\0') {
        total += *key;
        key++;
    }
    return total % TABLE_SIZE;
}

void insert(char *key, int value) {
    int index = hash(key);

    Node *new = calloc(1, sizeof(Node));
    strcpy(new->key, key);
    new->value = value;
    new->next = table[index];
    table[index] = new;
}

Node *lookup(char *key) {
    int index = hash(key);
    Node *current = table[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

void delete(char *key) {
    int index = hash(key);
    Node *prev = NULL;
    Node *current = table[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    return;
}

void print_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("[%d]", i);
        Node *current = table[i];
        while (current != NULL) {
            printf("-> %s:%d", current->key, current->value);
            current = current->next;
        }
        printf("-> NULL \n");
    }
}

void free_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table[i];
        while (current != NULL) {
            Node *temp = current->next;
            free(current);
            current = temp;
        }
        table[i] = NULL;
    }
}

void save_table(char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        perror("fopen:");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table[i];
        while (current != NULL) {
            fwrite(current->key, sizeof(current->key), 1, f);
            fwrite(&current->value, sizeof(current->value), 1, f);
            current = current->next;
        }
    }

    fclose(f);
}

void load_table(char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        perror("fopen:");
        return;
    }

    char key[32];
    int value;

    while (fread(key, sizeof(key), 1, f) == 1 && fread(&value, sizeof(value), 1, f) == 1) {
        insert(key, value);
    }
    fclose(f);
}

int main() {
    insert("fahim", 30);
    insert("aurin", 29);
    insert("ali", 30);
    insert("sara", 19);
    insert("nadia", 35);

    printf("--- original ---\n");
    print_table();

    save_table("table.bin");
    free_table();

    printf("--- after save and free ---\n");
    print_table();

    load_table("table.bin");
    printf("--- after loading from file ---\n");
    print_table();

    free_table();
    return 0;
}
