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

void insert(Node **root, int value) {

    if (*root == NULL) {
        Node *new = malloc(sizeof(Node));
        new->value = value;
        new->left = NULL;
        new->right = NULL;
        *root = new;
        return;
    }

    if (value < (*root)->value) {
        insert(&(*root)->left, value);
    }

    if (value > (*root)->value) {
        insert(&(*root)->right, value);
    }
}

void inorder(Node *root) {
    if (root == NULL)
        return;

    inorder(root->left);
    printf("%d\n", root->value);
    inorder(root->right);
}

int main() {
    Node *root = NULL;
    insert(&root, 10);
    insert(&root, 5);
    insert(&root, 20);
    insert(&root, 3);
    insert(&root, 7);

    inorder(root);

    return 0;
}
