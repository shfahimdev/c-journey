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

int main() {
    insert("abc", 1);
    insert("bca", 2);
    insert("cab", 3);

    printf("--- after inserting 3 colliding keys ---\n");
    print_table();

    printf("--- lookup all three ---\n");
    Node *a = lookup("abc");
    Node *b = lookup("bca");
    Node *c = lookup("cab");
    if (a)
        printf("found: %s = %d\n", a->key, a->value);
    if (b)
        printf("found: %s = %d\n", b->key, b->value);
    if (c)
        printf("found: %s = %d\n", c->key, c->value);

    printf("--- delete middle one ---\n");
    delete("bca");
    print_table();

    printf("--- lookup after delete ---\n");
    a = lookup("abc");
    c = lookup("cab");
    if (a)
        printf("still here: %s\n", a->key);
    if (c)
        printf("still here: %s\n", c->key);

    free_table();
    return 0;
}
