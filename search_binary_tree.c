#include <stdio.h>
#include <stdlib.h>
struct node_type {
    int info;
    struct node_type* left, * right;
};

typedef struct node_type* tree;

//פונקציית חיפוש בעץ חיפוש בינארי
tree search(tree t, int x) {
    if (t == NULL || t->info == x)
        return t;
    return t->info >= x ? search(t->right, x) : search(t->left, x);
}
//הכנסה
tree insert(tree t, int x) {
    if (t == NULL) {
        t = (tree)malloc(sizeof(struct node_type));
        t->info = x;
        t->left = t->right = NULL;
        return t;
    }
    if (t->info < x)
        t->right = insert(t->right, x);
    else
        t->left = insert(t->left, x);
    return t;
}
//סריקה pre order
void pre_order(tree t) {
    if (t != NULL) {
        printf("%d ",t->info);
        pre_order(t->left);
        pre_order(t->right);
    }
}
//פונקציה שמקבלת מערך ובונה עץ חיפוש
tree build(tree t, int arr[], int n) {
    
    if(n>0){
        t=insert(t, arr[0]);
        t = build(t, arr+1, n - 1);
    }
    return t;
}
//בדיקה האם הצומת הוא עלה
int is_leaf(tree t) {
    return (t->right == NULL && t->left == NULL);
}
tree find_min(tree t) {
    if (t->left != NULL)
        t = find_min(t->left);
    return t;
}
tree find_max(tree t) {
    if (t->right != NULL)
        t = find_max(t->right);
    return t;
}
tree find_parent(tree t, int x) {
    if (t == NULL||t->left==NULL&&t->right==NULL)
        return NULL;
    if (t->left!=NULL&&(t->left)->info > x)
        t = find_parent(t->left, x);
    else if(t->right != NULL&& (t->right)->info < x)
       t = find_parent(t->right, x);
    return t;
}
tree next_value(tree t, int x) {
    if (t == NULL || x == find_max(t))
        return NULL;
    if (t->right != NULL)
        return find_min(search(t,x)->right);
    tree y = find_parent(t, x);
    while ( y!= NULL && x == y->right->info) {
        x = y;
        y = find_parent(t, x);
    }
    return y;
}
tree prev_value(tree t, int x) {
    if (t == NULL || x == find_max(t))
        return NULL;
    if (t->left != NULL)
        return find_max(search(t,x)->left);
    return find_parent(t, x);

}
//tree delete(tree t, int x) {
//    if (t == NULL)
//        return t;
//    if (!is_leaf(t)&&t->info!=x) {
//        t->left = delete(t->left, x);
//        t->right = delete(t->right, x);
//    }
//    else
//        if (t->info == x) {
//            t = NULL;
//        }
//
//
//}

// tree rr
tree RR(tree problem) {
    tree t = problem->right;
    tree tmp = t->left;
    t->left = problem;
    problem->right = tmp;
    return t;
}

//tree LL
tree LL(tree problem) {
    tree new_tree = problem->left;
    tree right_tmp = new_tree->right;
    new_tree->right = problem;
    problem->right = right_tmp;
    return new_tree;
}

tree RL(tree problem) {
    tree new_root = problem->right->left;
    tree left, right;
    left = new_root->left;
    right = new_root->right;
    new_root->right = problem->right;
    new_root->left = problem;
    problem->right = left;
    new_root->right->left = right;
    return new_root;
}

tree LR(tree problem) {
    tree new_root = problem->left->right;
    tree left, right;
    left = new_root->left;
    right = new_root->right;
    new_root->left = problem->left;
    new_root->right = problem;
    problem->left = right;
    new_root->left->right = left;
    return new_root;
}

void main() {
    tree t = NULL;
    int arr[] = { 10, 14, 5, 8, 56, 13, 1 };
    int n = 7;
    t = build(t, arr, n);
    pre_order(t);
    //printf("\nthe max value is %d ", (find_max(t))->info);
    printf("\n%d ", (find_parent(t, 1))->info);
    printf("\n%d ", (next_value(t, 13))->info);
}