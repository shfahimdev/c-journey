#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 16

typedef struct {
    char name[32];
    char phone[16];
} Contact;

typedef struct Node {
    Contact data;
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

void add(char *name, char *phone) {
    int index = hash(name);

    Node *new = calloc(1, sizeof(Node));
    strcpy(new->data.name, name);
    strcpy(new->data.phone, phone);
    new->next = table[index];
    table[index] = new;
}

Node *search(char *name) {
    int index = hash(name);
    Node *current = table[index];
    while (current != NULL) {
        if (strcmp(current->data.name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void delete(char *name) {
    int index = hash(name);
    Node *prev = NULL;
    Node *current = table[index];

    while (current != NULL) {
        if (strcmp(current->data.name, name) == 0) {
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

void table_print() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table[i];
        while (current != NULL) {
            printf("%s - %s\n", current->data.name, current->data.phone);
            current = current->next;
        }
    }
    return;
}

void save(char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        perror("fopen");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table[i];
        while (current != NULL) {
            fwrite(&current->data, sizeof(Contact), 1, f);
            current = current->next;
        }
    }
    fclose(f);
    return;
}

void load(char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        return;
    }

    Contact c;
    while (fread(&c, sizeof(Contact), 1, f) == 1) {
        add(c.name, c.phone);
    }
    fclose(f);
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
    load("contacts.bin");
    int choice;
    char name[32];
    char phone[16];

    while (1) {
        printf("\n1. Add\n2. Search\n3. Delete\n4. List all\n5. Exit\n> ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Name: ");
            scanf("%s", name);
            printf("Phone: ");
            scanf("%s", phone);
            add(name, phone);
            printf("Contact added.\n");
            break;

        case 2:
            printf("Name: ");
            scanf("%s", name);
            Node *result = search(name);
            if (result != NULL)
                printf("Found: %s - %s\n", result->data.name, result->data.phone);
            else
                printf("Not found.\n");
            break;

        case 3:
            printf("Name: ");
            scanf("%s", name);
            delete(name);
            printf("Deleted (if existed).\n");
            break;

        case 4:
            table_print();
            break;

        case 5:
            save("contacts.bin");
            free_table();
            return 0;

        default:
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
