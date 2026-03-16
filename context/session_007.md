# Session 007 — BST Deletion

**Date:** 2026-03-16
**Duration:** ~1 session
**Folder:** `07-bst-deletion/`

---

## What We Covered

### The Three Cases of BST Deletion

**Case 1 — leaf node (no children)**
Free the node, set `*root = NULL` to patch the parent.

**Case 2 — one child**
Save the node in `temp`, point `*root` at the child, free `temp`.

**Case 3 — two children (the hard case)**
Don't remove the node directly. Instead:
1. Find the inorder successor — `find_min((*root)->right)`
2. Copy the successor's value into the target node
3. Recursively delete the successor from the right subtree

The successor is always the leftmost node in the right subtree. It will always be a leaf or have one child, so it reduces to Case 1 or 2.

### Why the Inorder Successor?
- Every value in the right subtree is larger than the target
- The smallest of those is the next value up in sorted order
- Replacing the target with it preserves the BST property:
  - left subtree is still smaller
  - right subtree is still larger

### Why the Successor is Safe to Delete
The leftmost node in any subtree has no left child by definition.
So it's always a leaf or has only a right child — both easy cases.

### find_min
```c
Node *find_min(Node *root) {
    if (root == NULL)       return NULL;
    if (root->left == NULL) return root;
    return find_min(root->left);
}
```

### Full Delete Function
```c
void delete(Node **root, int value) {
    Node *current = *root;
    if (current == NULL) return;

    if (current->value == value) {

        // two children — inorder successor trick
        if (current->right != NULL && current->left != NULL) {
            Node *successor = find_min(current->right);
            current->value = successor->value;
            delete(&current->right, successor->value);
            return;
        }

        // one or no children — bypass
        if (current->right != NULL)      *root = current->right;
        else if (current->left != NULL)  *root = current->left;
        else                             *root = NULL;

        free(current);
        return;
    }

    if (current->value > value) delete(&current->left,  value);
    if (current->value < value) delete(&current->right, value);
}
```

---

## Gotchas Hit
- `find_min(current)` instead of `find_min(current->right)` — finds min of whole subtree, not the successor
- Freeing the successor directly instead of recursively deleting it — leaves a dangling pointer in the parent
- Reading `temp->right` after `free(temp)` — always save the child before freeing
- One-child case: recursing into the child instead of bypassing the node — doesn't remove the target

---

## Files Written
- `01-delete.c` — delete a leaf node
- `02-delete.c` — delete a node with one child
- `03-delete.c` — full delete, all three cases, valgrind clean

---

## Stress Test (03-delete.c)
- Delete a leaf ✓
- Delete a node with one child ✓
- Delete a node with two children ✓
- Delete the root ✓
- Delete a nonexistent value ✓
- Valgrind: 9 allocs, 9 frees, 0 errors ✓

---

## Where We Stopped
Finished `07-bst-deletion`. Full BST deletion implemented and valgrind clean.

## What's Next
- **Hash tables** — arrays + linked lists combined, O(1) average lookup
- **Binary tree serialization** — save a tree to file, reconstruct it
- **Practice project** — contact book using a BST
