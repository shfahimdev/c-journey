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

void delete(Node **head, int value) {

    if (head == NULL)
        return;

    if ((*head)->value == value) {
        Node *temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }

    Node *prev = *head;
    Node *current = (*head)->next;

    while (current->next != NULL) {
        if (current->value == value) {
            prev->next = current->next;
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
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

    printf("-------------\n");

    delete(&head, 20);
    print_list(head);

    printf("-------------\n");

    delete(&head, 10);
    print_list(head);

    free(head);
    head = NULL;

    return 0;
}
