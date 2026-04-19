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
    ERROR_INVALID_ARGUMENT = -7,
    NODE_ALREADY_EXISTS = 1,
}status_codes;

/* Creazione / distruzione */
status_codes avl_create(size_t sizeOfVals,AVLTree* tree);
status_codes avl_destroy(AVLTree* tree);

/* Operazioni base */
status_codes avl_insert(AVLTree tree,int (*cmp)(const void*,const void*), void* value);
status_codes avl_remove(AVLTree tree,int (*cmp)(const void*, const void*) ,void* value);
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
/**
 * @brief Stampa l'albero AVL in forma ruotata di 90 gradi.
 *
 * La stampa mostra prima il sottoalbero destro, poi il nodo corrente,
 * poi il sottoalbero sinistro, così da visualizzare la struttura
 * gerarchica dell'albero nel terminale.
 *
 * @param tree albero da stampare
 * @param print_value funzione callback usata per stampare il valore contenuto in un nodo
 *
 * @retval ERROR_NULL_POINTER se tree o print_value sono NULL
 * @retval OK stampa completata con successo
 */
status_codes avl_print(AVLTree tree, void (*print_value)(const void*));

/**
 * @brief Stampa solo i valori dell'albero in ordine crescente (visita inorder).
 *
 * @param tree albero da stampare
 * @param print_value funzione callback usata per stampare il valore contenuto in un nodo
 *
 * @retval ERROR_NULL_POINTER se tree o print_value sono NULL
 * @retval OK stampa completata con successo
 */
status_codes avl_print_inorder(AVLTree tree, void (*print_value)(const void*));


#endif //LIBFEM_BST_AVL_H