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

Node *search(Node *root, int value) {
  if (root == NULL) {
    printf("Not Found\n");
    return NULL;
  }

  if (value == root->value) {
    return root;
  }

  if (value < root->value) {
    return search(root->left, value);
  } else {
    return search(root->right, value);
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

Node *find_max(Node *root) {
  if (root == NULL) {
    return NULL;
  }
  if (root->right == NULL) {
    return root;
  } else {
    return find_max(root->right);
  }
}

void freeTree(Node *root) {
  if (root == NULL) return;

  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

void inorder(Node *root) {
  if (root == NULL) return;

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
  insert(&root, 1); 

  Node *searchedNode = search(root, 3);
  printf("Found %d in the Tree\n", searchedNode->value);

  Node *minimumNode = find_min(root);
  printf("%d is the minimum value\n", minimumNode->value);

  Node *maximumNode = find_max(root);
  printf("%d is the maximum value\n", maximumNode->value);

  freeTree(root);
 
  return 0;
}
