#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

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

Node *find_min(Node *root) {
    if (root == NULL) {
        return NULL;
    }

    if (root->left == NULL) {
        return root;
    } else {
        return find_min(root->left);
    }
}

void delete_leaf(Node **root, int value) {
    Node *current = *root;
    if (current == NULL)
        return;

    if (current->value == value) {

        if (current->right != NULL && current->left != NULL) {
            Node *successor = find_min(current->right);
            current->value = successor->value;
            delete_leaf(&current->right, successor->value);
            return;
        }

        if (current->right != NULL) {
            *root = current->right;
        } else if (current->left != NULL) {
            *root = current->left;
        } else {
            *root = NULL;
        }

        free(current);
        return;
    }

    if (current->value > value) {
        delete_leaf(&current->left, value);
    }

    if (current->value < value) {
        delete_leaf(&current->right, value);
    }
}

void freeTree(Node *root) {
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void inorder(Node *root) {
    if (root == NULL) {
        return;
    }

    inorder(root->left);
    printf("%d\n", root->value);
    inorder(root->right);
}

int main() {
    Node *root = NULL;
    insert(&root, 10);
    insert(&root, 8);
    insert(&root, 9);
    insert(&root, 7);
    insert(&root, 13);
    insert(&root, 14);
    insert(&root, 12);
    insert(&root, 11);

    inorder(root);

    printf("---------------------------------------\n");

    delete_leaf(&root, 7);
    inorder(root);

    printf("---------------------------------------\n");

    delete_leaf(&root, 8);
    inorder(root);

    printf("---------------------------------------\n");

    delete_leaf(&root, 13);
    inorder(root);

    printf("---------------------------------------\n");

    delete_leaf(&root, 10);
    inorder(root);

    printf("---------------------------------------\n");

    delete_leaf(&root, 69);
    inorder(root);

    freeTree(root);

    return 0;
}
