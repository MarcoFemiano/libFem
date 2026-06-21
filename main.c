//
// Created by Spara on 21/06/2026.
//

#include <stdio.h>

#include "BST_AVL.h"

int int_cmp(const void *a, const void *b) {
         const int *x = a;
         const int *y = b;

         if (*x < *y) return -1;if (*x > *y) return 1;
        return 0;
}

int main(void) {
    AVLTree tree;
    status_codes res;
    res = avl_create(&tree);

    for (int i = 0; i<100; i++) {
        res = avl_insert(tree,&int_cmp,&i);
        if (res != OK) return res;
    }

    printf("premi ok per partire\n");
    int a;
    scanf("%d ",&a);
    avl_DFS_attraversalMultiThread(tree,6);



    return 0;
}