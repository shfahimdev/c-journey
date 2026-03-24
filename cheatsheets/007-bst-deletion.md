# Cheatsheet — 07 BST Deletion

## The Three Cases

| Case | Condition | Action |
|------|-----------|--------|
| Leaf | no children | free, set `*root = NULL` |
| One child | one side is NULL | bypass node, free it |
| Two children | both sides non-NULL | inorder successor trick |

---

## find_min (inorder successor helper)
```c
Node *find_min(Node *root) {
    if (root == NULL)       return NULL;
    if (root->left == NULL) return root;
    return find_min(root->left);
}
```

---

## Full Delete
```c
void delete(Node **root, int value) {
    Node *current = *root;
    if (current == NULL) return;

    if (current->value == value) {

        if (current->right != NULL && current->left != NULL) {
            Node *successor = find_min(current->right);
            current->value = successor->value;
            delete(&current->right, successor->value);
            return;
        }

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

## Key Rules
- Always free **after** patching `*root` — never before
- Two children: copy the value up, delete the successor down
- Inorder successor = `find_min(right subtree)` — always leftmost
- Successor is safe to delete — it has at most one child (right only)

---

## Gotchas
- `find_min(current)` finds min of whole subtree — you want `find_min(current->right)`
- Freeing successor directly orphans the parent's pointer — recurse to delete it instead
- Always save child pointer before freeing the node
```

---

## Commit message
```
session 007: BST deletion — all three cases, valgrind clean
