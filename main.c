//
// Created by Spara on 21/06/2026.
//

#include <stdio.h>

#include "BST_AVL.h"

int int_cmp(const void *a, const void *b) {
         const int *x = a;
         const int *y = b;

         if (*x < *y) return -1;
         if (*x > *y) return 1;
        return 0;
}

int main(void) {
    AVLTree tree = NULL;
    status_codes res;
    res = avl_create(&tree);
    if (res != OK) return res;

    int values[100];
    for (int i = 0; i<100; i++) {
        values[i] = i;
        res = avl_insert(tree,&int_cmp,&values[i]);
        if (res != OK) {
            avl_destroy(&tree);
            return res;
        }
    }

    printf("premi ok per partire\n");
    int a;
    if (scanf("%d",&a) != 1) {
        avl_destroy(&tree);
        return ERROR_INVALID_ARGUMENT;
    }
    avl_DFS_attraversalMultiThread(tree,6);

    avl_destroy(&tree);

    return 0;
}
