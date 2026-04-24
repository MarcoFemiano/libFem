/**
 * @file BST_AVL.h
 * @brief ADT AVL/BST generico con dati arbitrari gestiti tramite puntatori `void *`.
 *
 * Questo modulo implementa un albero binario di ricerca autobilanciato (AVL).
 * I valori memorizzati sono generici: l'albero salva semplicemente il puntatore
 * ricevuto in input e usa una funzione di confronto fornita dall'utilizzatore
 * per mantenere l'ordinamento.
 *
 * -----------------------------------------------------------------------------
 * USO SEMPLICE DELL'ADT
 * -----------------------------------------------------------------------------
 *
 * 1) Creare l'albero:
 * @code
 * AVLTree tree;
 * status_codes res = avl_create(&tree);
 * @endcode
 *
 * 2) Preparare dei dati da inserire.
 *    L'albero salva il puntatore passato a `avl_insert()`.
 *    Quindi il dato deve restare valido finché rimane dentro l'albero.
 *
 *    Esempio con interi allocati dinamicamente:
 * @code
 * int *a = malloc(sizeof(int));
 * int *b = malloc(sizeof(int));
 * *a = 10;
 * *b = 20;
 * @endcode
 *
 * 3) Scrivere una funzione `cmp` che dica quale valore è minore, uguale o maggiore.
 *    La funzione deve restituire:
 *    - -1 se il primo elemento è minore del secondo
 *    - 0 se i due elementi sono uguali
 *    - 1 se il primo elemento è maggiore del secondo
 *    - -2 per segnalare errori nell'esecuzione
 *
 *    Esempio per `int`:
 * @code
 * int int_cmp(const void *a, const void *b) {
 *     const int *x = a;
 *     const int *y = b;
 *
 *     if (*x < *y) return -1;
 *     if (*x > *y) return 1;
 *     return 0;
 * }
 * @endcode
 *
 *    Regola importante:
 *    la `cmp` deve confrontare il CONTENUTO logico dei dati, non gli indirizzi,
 *    a meno che tu non voglia davvero ordinare i puntatori per indirizzo.
 *
 * 4) Inserire i valori:
 * @code
 * avl_insert(tree, int_cmp, a);
 * avl_insert(tree, int_cmp, b);
 * @endcode
 *
 * 5) Cercare un valore:
 * @code
 * bool found;
 * int key = 10;
 * avl_search(tree, &key, int_cmp, &found);
 * @endcode
 *
 * 6) Ottenere informazioni:
 * @code
 * size_t n;
 * int h;
 * avl_size(tree, &n);
 * avl_height(tree, &h);
 * @endcode
 *
 * 7) Stampare i valori.
 *    Serve una callback che sappia stampare un singolo dato:
 * @code
 * void print_int(const void *p) {
 *     printf("%d", *(const int *)p);
 * }
 *
 * avl_print(tree, print_int);
 * avl_print_inorder(tree, print_int);
 * @endcode
 *
 * 8) Distruggere l'albero:
 * @code
 * avl_destroy(&tree);
 * @endcode
 *
 * -----------------------------------------------------------------------------
 * GESTIONE DELLA MEMORIA
 * -----------------------------------------------------------------------------
 *
 * Questo ADT libera i nodi interni dell'albero, ma NON libera i dati puntati
 * da `void *val`.
 *
 * Quindi:
 * - se inserisci puntatori a memoria allocata dinamicamente, sei tu a doverla
 *   liberare al momento opportuno;
 * - `avl_destroy()` libera la struttura dell'albero e i nodi, non il contenuto
 *   applicativo dei nodi.
 *
 * -----------------------------------------------------------------------------
 * DUPLICATI
 * -----------------------------------------------------------------------------
 *
 * Se la funzione `cmp` considera due valori uguali, il secondo inserimento
 * viene rifiutato e la funzione ritorna `NODE_ALREADY_EXISTS`.
 *
 * -----------------------------------------------------------------------------
 * CONVENZIONE SULL'ALTEZZA
 * -----------------------------------------------------------------------------
 *
 * In questa implementazione:
 * - un albero vuoto ha altezza -1
 * - una foglia ha altezza 0
 *
 * -----------------------------------------------------------------------------
 * ESEMPIO COMPLETO MINIMO
 * -----------------------------------------------------------------------------
 * @code
 * #include "BST_AVL.h"
 * #include <stdio.h>
 * #include <stdlib.h>
 *
 * int int_cmp(const void *a, const void *b) {
 *     const int *x = a;
 *     const int *y = b;
 *     if (*x < *y) return -1;
 *     if (*x > *y) return 1;
 *     return 0;
 * }
 *
 * void print_int(const void *p) {
 *     printf("%d", *(const int *)p);
 * }
 *
 * int main(void) {
 *     AVLTree tree;
 *     if (avl_create(&tree) != OK) return 1;
 *
 *     int *v1 = malloc(sizeof(int));
 *     int *v2 = malloc(sizeof(int));
 *     int *v3 = malloc(sizeof(int));
 *     *v1 = 30;
 *     *v2 = 10;
 *     *v3 = 20;
 *
 *     avl_insert(tree, int_cmp, v1);
 *     avl_insert(tree, int_cmp, v2);
 *     avl_insert(tree, int_cmp, v3);
 *
 *     avl_print_inorder(tree, print_int);
 *
 *     avl_destroy(&tree);
 *
 *     free(v1);
 *     free(v2);
 *     free(v3);
 *     return 0;
 * }
 * @endcode
 */

#ifndef LIBFEM_BST_AVL_H
#define LIBFEM_BST_AVL_H

#include <stddef.h>

/** @brief Tipo booleano minimale usato dal modulo. */
typedef char bool;

/** @brief Valore booleano vero. */
#define true 1
/** @brief Valore booleano falso. */
#define false 0

/**
 * @brief Handle opaco dell'albero AVL.
 *
 * L'utilizzatore vede solo un puntatore a struttura incompleta.
 * I dettagli interni sono nascosti nel file `.c`.
 */
typedef struct strAVLTree* AVLTree;

/**
 * @brief Alias del tipo nodo interno.
 *
 * Questo tipo è esposto solo come nome di puntatore, ma la struttura concreta
 * resta privata all'implementazione.
 */
typedef struct strNodo* Nodo;

/**
 * @brief Codici di stato restituiti dalle funzioni dell'ADT.
 */
typedef enum {
    /** Operazione completata con successo. */
    OK = 0,
    /** Uno o più puntatori obbligatori sono NULL. */
    ERROR_NULL_POINTER = -1,
    /** Overflow aritmetico durante un calcolo interno. */
    ERROR_ARITHMETIC_OVERFLOW = -2,
    /** Fallimento di una realloc. */
    ERROR_REALLOC_FAILURE = -3,
    /** Fallimento di una allocazione dinamica. */
    ERROR_ALLOCATION_FAILURE = -4,
    /** Operazione non valida perché l'albero è vuoto. */
    ERROR_EMPTY_TREE = -5,
    /** Elemento richiesto non trovato. */
    ERROR_NOT_FOUND = -6,
    /** Argomento formalmente valido ma semanticamente non accettabile. */
    ERROR_INVALID_ARGUMENT = -7,
    /** Inserimento rifiutato perché il nodo esiste già secondo la `cmp`. */
    NODE_ALREADY_EXISTS = 1,
} status_codes;

/**
 * @brief Crea un nuovo albero AVL vuoto.
 *
 * Alloca la struttura contenitore dell'albero, inizializza la radice a `NULL`,
 * la dimensione a 0 e l'altezza logica a -1.
 *
 * @param[out] tree indirizzo della variabile che riceverà l'handle dell'albero
 *
 * @retval OK creazione completata con successo
 * @retval ERROR_NULL_POINTER `tree` è NULL
 * @retval ERROR_ALLOCATION_FAILURE impossibile allocare la struttura dell'albero
 */
status_codes avl_create(AVLTree* tree);

/**
 * @brief Distrugge completamente l'albero e tutti i suoi nodi.
 *
 * La funzione libera la struttura interna dell'albero e tutti i nodi allocati,
 * ma NON libera i valori puntati dai campi `void *` contenuti nei nodi.
 *
 * Dopo la distruzione, `*tree` viene impostato a `NULL`.
 *
 * @param[in,out] tree indirizzo dell'handle dell'albero da distruggere
 *
 * @retval OK distruzione completata con successo
 * @retval ERROR_NULL_POINTER `tree` è NULL oppure `*tree` è NULL
 */
status_codes avl_destroy(AVLTree* tree);

/**
 * @brief Inserisce un nuovo valore nell'albero.
 *
 * L'inserimento rispetta l'ordinamento definito dalla funzione `cmp`.
 * Se un valore equivalente è già presente, l'inserimento viene rifiutato.
 *
 * Il puntatore `value` viene memorizzato così com'è, senza copiarne il contenuto.
 *
 * @param[in] tree albero su cui inserire il nuovo valore
 * @param[in] cmp funzione di confronto tra due valori
 * @param[in] value puntatore al valore da inserire
 *
 * @retval OK inserimento completato con successo
 * @retval ERROR_NULL_POINTER `tree`, `cmp` o `value` sono NULL
 * @retval ERROR_ALLOCATION_FAILURE impossibile allocare un nuovo nodo
 * @retval NODE_ALREADY_EXISTS un valore equivalente è già presente
 */
status_codes avl_insert(AVLTree tree, int (*cmp)(const void*, const void*), void* value);

/**
 * @brief Rimuove un valore dall'albero.
 *
 * Il nodo da eliminare viene individuato usando la funzione `cmp`.
 * Se il nodo esiste, la struttura dell'albero viene aggiornata e riequilibrata.
 *
 * La funzione rimuove e libera il nodo, ma NON libera il dato puntato da `value`
 * né il dato effettivamente contenuto nel nodo.
 *
 * @param[in] tree albero da cui rimuovere il valore
 * @param[in] cmp funzione di confronto tra due valori
 * @param[in] value chiave logica da cercare e rimuovere
 *
 * @retval OK rimozione completata con successo
 * @retval ERROR_NULL_POINTER `tree`, `cmp` o `value` sono NULL
 * @retval ERROR_NOT_FOUND nessun nodo equivalente è stato trovato
 */
status_codes avl_remove(AVLTree tree, int (*cmp)(const void*, const void*), void* value);

/**
 * @brief Cerca un valore nell'albero.
 *
 * Il confronto è effettuato tramite la callback `cmp`.
 * Il risultato della ricerca viene scritto in `*result`.
 *
 * @param[in] tree albero in cui effettuare la ricerca
 * @param[in] value chiave logica da cercare
 * @param[in] cmp funzione di confronto tra due valori
 * @param[out] result riceve `true` se il valore è presente, `false` altrimenti
 *
 * @retval OK ricerca completata con successo
 * @retval ERROR_NULL_POINTER `tree`, `value`, `cmp` o `result` sono NULL
 */
status_codes avl_search(AVLTree tree, const void* value, int (*cmp)(const void*, const void*), bool* result);

/**
 * @brief Verifica se l'albero è vuoto.
 *
 * @param[in] tree albero da controllare
 * @param[out] result riceve `true` se l'albero è vuoto, `false` altrimenti
 *
 * @retval OK operazione completata con successo
 * @retval ERROR_NULL_POINTER `tree` o `result` sono NULL
 */
status_codes avl_is_empty(AVLTree tree, bool* result);

/**
 * @brief Restituisce il numero di nodi presenti nell'albero.
 *
 * @param[in] tree albero di cui leggere la dimensione
 * @param[out] result riceve il numero di nodi presenti
 *
 * @retval OK operazione completata con successo
 * @retval ERROR_NULL_POINTER `tree` o `result` sono NULL
 */
status_codes avl_size(AVLTree tree, size_t* result);

/**
 * @brief Restituisce l'altezza dell'albero.
 *
 * Convenzione usata:
 * - albero vuoto -> -1
 * - foglia -> 0
 *
 * @param[in] tree albero di cui leggere l'altezza
 * @param[out] result riceve l'altezza calcolata
 *
 * @retval OK operazione completata con successo
 * @retval ERROR_NULL_POINTER `tree` o `result` sono NULL
 */
status_codes avl_height(AVLTree tree, int* result);

/**
 * @brief Restituisce il valore minimo presente nell'albero.
 *
 * Il valore minimo è quello contenuto nel nodo più a sinistra.
 * La funzione restituisce il puntatore al dato memorizzato, non una sua copia.
 *
 * @param[in] tree albero da interrogare
 * @param[out] result riceve il puntatore al valore minimo
 *
 * @retval OK operazione completata con successo
 * @retval ERROR_NULL_POINTER `tree` o `result` sono NULL
 * @retval ERROR_EMPTY_TREE l'albero è vuoto
 */
status_codes avl_min(AVLTree tree, void** result);

/**
 * @brief Restituisce il valore massimo presente nell'albero.
 *
 * Il valore massimo è quello contenuto nel nodo più a destra.
 * La funzione restituisce il puntatore al dato memorizzato, non una sua copia.
 *
 * @param[in] tree albero da interrogare
 * @param[out] result riceve il puntatore al valore massimo
 *
 * @retval OK operazione completata con successo
 * @retval ERROR_NULL_POINTER `tree` o `result` sono NULL
 * @retval ERROR_EMPTY_TREE l'albero è vuoto
 */
status_codes avl_max(AVLTree tree, void** result);

/**
 * @brief Verifica se tutti i nodi dell'albero rispettano il vincolo AVL.
 *
 * Il controllo viene eseguito leggendo il fattore di bilanciamento memorizzato
 * in ogni nodo. La funzione è utile soprattutto per debug e test.
 *
 * @param[in] tree albero da controllare
 * @param[out] result riceve `true` se l'albero è bilanciato, `false` altrimenti
 *
 * @retval OK operazione completata con successo
 * @retval ERROR_NULL_POINTER `tree` o `result` sono NULL
 */
status_codes avl_is_balanced(AVLTree tree, bool* result);

/**
 * @brief Stampa l'albero AVL in forma ruotata di 90 gradi.
 *
 * La stampa mostra prima il sottoalbero destro, poi il nodo corrente,
 * poi il sottoalbero sinistro, così da visualizzare la struttura
 * gerarchica dell'albero nel terminale.
 *
 * @param[in] tree albero da stampare
 * @param[in] print_value funzione callback usata per stampare il valore contenuto in un nodo
 *
 * @retval ERROR_NULL_POINTER se `tree` o `print_value` sono NULL
 * @retval OK stampa completata con successo
 */
status_codes avl_print(AVLTree tree, void (*print_value)(const void*));

/**
 * @brief Stampa solo i valori dell'albero in ordine crescente (visita inorder).
 *
 * La stampa sfrutta una visita inorder, quindi l'output risulta ordinato
 * secondo la relazione definita da `cmp`.
 *
 * @param[in] tree albero da stampare
 * @param[in] print_value funzione callback usata per stampare il valore contenuto in un nodo
 *
 * @retval ERROR_NULL_POINTER se `tree` o `print_value` sono NULL
 * @retval OK stampa completata con successo
 */
status_codes avl_print_inorder(AVLTree tree, void (*print_value)(const void*));

#endif /* LIBFEM_BST_AVL_H */
