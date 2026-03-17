#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

void append(Node **head, int value) {
    Node *new = malloc(sizeof(Node));
    new->value = value;
    new->next = NULL;

    if (*head == NULL) {
        *head = new;
        return;
    }
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = new;
    return;
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d\n", current->value);
        current = current->next;
    }
}

int main() {
    Node *head = NULL;

    append(&head, 10);
    append(&head, 20);
    append(&head, 30);

    print_list(head);

    free(head);
    head = NULL;

    return 0;
}
