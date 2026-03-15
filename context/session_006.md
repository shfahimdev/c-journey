# Session 006 — Binary Trees

**Date:** 2026-03-15
**Duration:** ~1 session
**Folder:** `06-binary-trees/`

---

## What We Covered

### What is a Binary Tree?
- A tree is a collection of nodes where each node has at most two children: `left` and `right`
- The top node is called the **root**
- A node with no children is called a **leaf**
- Unlike a linked list (one `next`), each node has two pointers

```c
typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;
```

### create_node
- Same pattern as linked list node creation — malloc, set fields, null both children, return pointer

```c
Node *create_node(int num) {
    Node *new = malloc(sizeof(Node));
    new->value = num;
    new->left  = NULL;
    new->right = NULL;
    return new;
}
```

### Traversals
- A tree has no single natural order — you choose how to walk it
- All three follow the same skeleton, only the position of printf changes

```
inorder:    left → PRINT → right   → sorted output on a BST
preorder:   PRINT → left → right   → root always comes first
postorder:  left → right → PRINT   → children before parent (use for freeing)
```

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

### Binary Search Tree (BST)
- A BST enforces a rule: left child < parent < right child
- This makes search, insert, min, max all O(log n) on a balanced tree
- Inorder traversal of a BST always produces sorted output

### Insert
- Takes `Node **root` — double pointer needed to modify head when tree is empty
- Recursively walks left or right based on value comparison
- Creates a node only when it reaches a NULL slot

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

### Search
- Takes `Node *` (read-only, no need to modify the tree)
- Returns the node if found, NULL if not
- Follows BST rule to eliminate half the tree at each step

```c
Node *search(Node *root, int value) {
    if (root == NULL) return NULL;
    if (value == root->value) return root;
    if (value < root->value) return search(root->left,  value);
    else                     return search(root->right, value);
}
```

### find_min / find_max
- Min = keep going left until you can't
- Max = keep going right until you can't

```c
Node *find_min(Node *root) {
    if (root == NULL)        return NULL;
    if (root->left == NULL)  return root;
    return find_min(root->left);
}

Node *find_max(Node *root) {
    if (root == NULL)        return NULL;
    if (root->right == NULL) return root;
    return find_max(root->right);
}
```

### free_tree
- Must free children before parent — postorder
- Setting `root = NULL` inside the function does nothing — it's a local copy
- To null the caller's pointer you'd need `Node **root`

```c
void free_tree(Node *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
```

---

## The Big Idea — Recursive Thinking
- Every recursive function has two parts:
  - **Base case** — `if (root == NULL) return;` — stops the recursion
  - **Recursive case** — call yourself on a smaller problem (left or right subtree)
- Same function, smaller problem, until you hit the base case, then unwind

---

## Gotchas Hit
- Calling `inorder` inside `preorder` and `postorder` instead of the function's own name — recursive calls must call themselves
- `root = NULL` after `free(root)` inside a function does nothing — only sets the local copy
- Mallocing at the top of insert before checking NULL — only malloc when you've found the right empty slot

---

## Files Written
- `01-binary-tree.c` — Node struct, create_node, manual tree wiring
- `02-binary-tree.c` — inorder, preorder, postorder traversals
- `03-binary-tree.c` — BST insert, build tree dynamically
- `04-binary-tree.c` — search, find_min, find_max
- `05-binary-tree.c` — free_tree, valgrind clean (7 allocs, 7 frees)

---

## Where We Stopped
Finished `06-binary-trees`. All 5 exercises done. Valgrind clean.

## What's Next
- **Hash tables** — arrays + linked lists combined, O(1) average lookup
- **Binary tree deletion** — the hard case (deleting a node with two children)
- **Practice project** — contact book using a BST instead of a linked list
