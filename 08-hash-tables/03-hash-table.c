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

    Node *new = malloc(sizeof(Node));
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

int main() {
    insert("fahim", 30);
    insert("fahiq", 25);
    insert("aurin", 29);
    insert("abid", 18);
    insert("shoyeb", 20);
    insert("ali", 30);
    insert("sara", 19);
    insert("nadia", 35);

    print_table();

    printf("------------------------------\n");

    delete("fahim");
    delete("ghost");
    delete("nadia");

    print_table();
    return 0;
}
