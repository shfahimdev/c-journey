#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[32];
    int health;
} Player;

typedef struct Node {
    Player data;
    struct Node *next;
} Node;

void append(Node **head, char name[], int health) {
    Node *new = malloc(sizeof(Node));
    strcpy(new->data.name, name);
    new->data.health = health;
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
        printf("%s - health : %d\n", current->data.name, current->data.health);
        current = current->next;
    }
}

int main() {
    Node *head = NULL;

    append(&head, "Fahim", 80);
    append(&head, "Aurin", 100);
    append(&head, "Aurifa", 20);

    FILE *f = fopen("players.bin", "wb");

    Node *current = head;
    while (current != NULL) {
        fwrite(&current->data, sizeof(Player), 1, f);
        current = current->next;
    }
    fclose(f);

    f = fopen("players.bin", "rb");
    Node *another_head = NULL;
    Player data;
    while ((fread(&data, sizeof(Player), 1, f) == 1)) {
        append(&another_head, data.name, data.health);
    }
    fclose(f);

    print_list(another_head);

    return 0;
}
