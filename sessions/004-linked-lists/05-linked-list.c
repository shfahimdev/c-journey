#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    ALIVE,
    DEAD,
    UNKNOWN,
} Status;

typedef struct {
    char name[32];
    Status status;
} Player;

typedef struct Node {
    Player data;
    struct Node *next;
} Node;

void append(Node **head, char name[], Status status) {
    Node *new = malloc(sizeof(Node));
    strcpy(new->data.name, name);
    new->data.status = status;
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

void delete(Node **head, char name[]) {

    if (head == NULL)
        return;

    if (strcmp((*head)->data.name, name) == 0) {
        Node *temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }

    Node *prev = *head;
    Node *current = (*head)->next;

    while (current->next != NULL) {
        if (strcmp(current->data.name, name) == 0) {
            prev->next = current->next;
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

const char *status_to_string(Status s) {
    switch (s) {
    case ALIVE:
        return "is Alive";
    case DEAD:
        return "is Dead";
    case UNKNOWN:
        return "is somewhere Fucked!";
    default:
        return "invalid";
    }
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%s %s\n", current->data.name, status_to_string(current->data.status));
        current = current->next;
    }
}

int main() {
    Node *head = NULL;

    append(&head, "Fahim", ALIVE);
    append(&head, "Aurin", DEAD);
    append(&head, "Abid", UNKNOWN);

    print_list(head);

    printf("---------------------\n");

    delete(&head, "Aurin");

    print_list(head);

    return 0;
}
