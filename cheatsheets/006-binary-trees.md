# Cheatsheet — 06 Binary Trees

## Node Definition
```c
typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;
```

## create_node
```c
Node *create_node(int num) {
    Node *new = malloc(sizeof(Node));
    new->value = num;
    new->left  = NULL;
    new->right = NULL;
    return new;
}
```

---

## Traversals
| Order | Pattern | Use |
|-------|---------|-----|
| Inorder | left → print → right | sorted output on BST |
| Preorder | print → left → right | root first |
| Postorder | left → right → print | freeing a tree |

```c
void inorder(Node *root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->value);
    inorder(root->right);
}

void preorder(Node *root) {
    if (root == NULL) return;
    printf("%d ", root->value);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node *root) {
    if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->value);
}
```

---

## BST Insert
```c
void insert(Node **root, int value) {
    if (*root == NULL) {
        Node *new = malloc(sizeof(Node));
        new->value = value;
        new->left  = NULL;
        new->right = NULL;
        *root = new;
        return;
    }
    if (value < (*root)->value) insert(&(*root)->left,  value);
    if (value > (*root)->value) insert(&(*root)->right, value);
}
```
- Takes `Node **` — needs to modify root if tree is empty
- Only mallocs when it finds the right NULL slot

---

## Search
```c
Node *search(Node *root, int value) {
    if (root == NULL)          return NULL;
    if (value == root->value)  return root;
    if (value < root->value)   return search(root->left,  value);
    else                       return search(root->right, value);
}
```
- Takes `Node *` — read only, no modification needed
- Returns node pointer or NULL

---

## find_min / find_max
```c
Node *find_min(Node *root) {
    if (root == NULL)       return NULL;
    if (root->left == NULL) return root;
    return find_min(root->left);
}

Node *find_max(Node *root) {
    if (root == NULL)        return NULL;
    if (root->right == NULL) return root;
    return find_max(root->right);
}
```
- Min = leftmost node
- Max = rightmost node

---

## free_tree
```c
void free_tree(Node *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
```
- Always postorder — free children before parent
- Check with: `valgrind --leak-check=full ./program`

---

## Recursive Thinking
Every recursive function needs:
1. **Base case** — `if (root == NULL) return;` — stops the recursion
2. **Recursive case** — call yourself on left or right subtree

```
insert(root, 7) on this tree:
        10
       /
      5
        \
        [NULL] ← 7 lands here
```

---

## Gotchas
- Recursive calls must call **their own** function — not another traversal
- `root = NULL` after `free` inside a function is a no-op — sets local copy only
- Don't malloc at the top of insert — only malloc when you reach the NULL slot
- Inorder on a BST = sorted output. Always.
