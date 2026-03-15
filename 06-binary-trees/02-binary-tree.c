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

void inorder(Node *root) {
  if (root == NULL) return;

  inorder(root->left);
  printf("%d\n", root->value);
  inorder(root->right);
}

void preorder(Node *root) {
  if (root == NULL) return;

  printf("%d\n", root->value);
  preorder(root->left);
  preorder(root->right);
}

void postorder(Node *root) {
  if (root == NULL) return;

  postorder(root->left);
  postorder(root->right);
  printf("%d\n", root->value);
}


int main() {
  Node *root = create_node(10);
  
  root->left  = create_node(5);  
  root->right = create_node(20);

  printf("inorder:   "); inorder(root);   printf("\n");
  printf("preorder:  "); preorder(root);  printf("\n");
  printf("postorder: "); postorder(root); printf("\n");

  free(root->left);
  free(root->right);
  free(root);
  return 0;
}
