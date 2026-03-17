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

    Node *found = lookup("fahim");
    if (found)
        printf("found: %s = %d\n", found->key, found->value);

    Node *missing = lookup("ghost");
    if (missing == NULL)
        printf("ghost: not found\n");
    return 0;
}
