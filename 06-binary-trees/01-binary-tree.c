#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

Node *create_node(int num) {
    Node *new = malloc(sizeof(Node));
    new->value = num;
    new->left = NULL;
    new->right = NULL;

    return new;
}

int main() {
    Node *newNode = create_node(10);
    newNode->left = create_node(5);
    newNode->right = create_node(20);

    printf("root:  %d\n", newNode->value);
    printf("left:  %d\n", newNode->left->value);
    printf("right: %d\n", newNode->right->value);

    free(newNode->left);
    free(newNode->right);
    free(newNode);
    return 0;
}
