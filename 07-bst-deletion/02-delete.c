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

void delete_rightOrLeft_leaf(Node **root, int value) {
  if(*root == NULL) return;
  if((*root)->value == value) {
    Node *temp = *root; 

    if(temp->right != NULL) {
      *root = temp->right;
    } else if (temp->left != NULL) {
      *root = temp->left;
    } else {
      *root = NULL;
    }
    free(temp);
    return;
  }

  if((*root)->value > value) {
    delete_rightOrLeft_leaf(&(*root)->left, value);
  }

  if((*root)->value < value) {
    delete_rightOrLeft_leaf(&(*root)->right, value);
  }

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
  insert(&root, 5);
  insert(&root, 11);
  insert(&root, 18);
  insert(&root, 9);
  insert(&root, 2);

  inorder(root);

  printf("---------------------------------------\n");

  delete_rightOrLeft_leaf(&root, 18);
  inorder(root);

  return 0;
}
