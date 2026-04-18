//
// Created by Spara on 18/04/2026.
//

#ifndef LIBFEM_BST_AVL_H
#define LIBFEM_BST_AVL_H
#include <stddef.h>
typedef char bool;
#define true 1
#define false 0
typedef struct strAVLTree* AVLTree;
typedef struct strNodo* Nodo;
typedef enum {
    OK = 0,
    ERROR_NULL_POINTER = -1,
    ERROR_ARITHMETIC_OVERFLOW = -2,
    ERROR_REALLOC_FAILURE = -3,
    ERROR_ALLOCATION_FAILURE = -4,
    ERROR_EMPTY = -5,
    ERROR_NOT_FOUND = -6,
    ERROR_INVALID_ARGUMENT = -7
}status_codes;

/* Creazione / distruzione */
status_codes avl_create(size_t sizeOfVals,AVLTree* tree);
status_codes avl_destroy(AVLTree* tree);

/* Operazioni base */
status_codes avl_insert(AVLTree tree, void* value);
status_codes avl_remove(AVLTree tree, void* value);

status_codes avl_search(AVLTree tree,const void* value,int (*cmp)(const void*,const void*), bool* result);

/* Info struttura */
status_codes avl_is_empty(AVLTree tree, bool* result);
status_codes avl_size(AVLTree tree,size_t* result);
status_codes avl_height(AVLTree tree,size_t* result);

/* Min / Max */
status_codes avl_min(AVLTree tree,void* result);
status_codes avl_max(AVLTree tree,void* result);

/* Debug */
status_codes avl_is_balanced(AVLTree tree, bool* result);



#endif //LIBFEM_BST_AVL_H