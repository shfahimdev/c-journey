#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[32];
    char phone[16];
} Contact;

typedef struct Node {
    Contact data;
    struct Node *next;
} Node;

void append(Node **head, char name[] ,char phone[]) {
    Node *new = malloc(sizeof(Node));
    strcpy(new->data.name, name);
    strcpy(new->data.phone, phone);
    new->next = NULL;
    
    if(*head == NULL) {
        *head = new;
        return;
    }
    
    Node *current = *head;
    while(current->next != NULL) {
        current = current->next;
    }
    
    current->next = new;
    
    return;
}

void delete(Node **head, char name[]) {
   if(*head == NULL) return;

  if( strcmp( (*head)->data.name, name ) == 0 ) {
    Node *temp = *head;
    *head = (*head)->next;
    free(temp);
    return;
  }

  Node *prev = *head;
  Node *current = (*head)->next;

  while (current->next != NULL) {
    if( strcmp( current->data.name, name ) == 0 ) {
      prev->next = current->next;
      free(current);
      return;
    }
    prev = current;
    current = current->next;
  }
}

void find(Node *head, char name[]) {
    Node *current = head;
    while(current != NULL) {
        if (strcmp(current->data.name, name) == 0) {
            printf("Contact of %s Found!!! The Phone Number is %s\n", name, current->data.phone);
            return;
        }
        current = current->next;
    }
    printf("No Contact Found\n");
}

void print_all(Node *head) {
    Node *current = head;
    while(current != NULL) {
        printf("%s - %s \n", current->data.name, current->data.phone);
        current = current->next;
    }
    return;
}

int main() {
    Node *head = NULL;
    
    append(&head, "Aurin" ,"01324589381");
    append(&head, "Fahim" ,"01975169294");
    append(&head, "Beauty" ,"01309646461");
    append(&head, "Ifty" ,"01420420420");
    print_all(head);
    
    printf("Successfully appended everything\n");
    printf("--------------------------------\n");
    
    delete(&head, "Fahim");
    print_all(head);
    
    printf("Successfully deleted a Contact\n");
    printf("--------------------------------\n");
    
    find(head, "Ifty");
    
    printf("--------------------------------\n");
    
    return 0;
}
