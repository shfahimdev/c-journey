#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d\n", current->value);
        current = current->next;
    }
}

int main() {
    Node *a = malloc(sizeof(Node));
    a->value = 10;
    a->next = NULL;

    Node *b = malloc(sizeof(Node));
    b->value = 20;
    b->next = NULL;

    Node *c = malloc(sizeof(Node));
    c->value = 30;
    c->next = NULL;

    a->next = b;
    b->next = c;

    print_list(a);

    Node *current = a;
    while (current != NULL) {
        Node *temp = current->next;
        free(current);
        current = temp;
    }

    return 0;
}
