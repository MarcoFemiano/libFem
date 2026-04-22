//
// Created by Spara on 20/04/2026.
//

/**
 * @file hash_table.h
 * @brief API di una hash table generica con open addressing, linear probing e tombstone.
 *
 * # Guida rapida all'utilizzo
 *
 * Questa API implementa una hash table generica che memorizza gli elementi **by value**,
 * cioè copiando i byte dell'oggetto passato dall'utente all'interno della tabella.
 *
 * ## Come funziona in pratica
 * - la tabella usa `XXH3_64bits` per calcolare l'hash dell'elemento;
 * - le collisioni vengono gestite con **linear probing**;
 * - la rimozione usa uno stato **deleted** (tombstone), così non si rompe la ricerca;
 * - la tabella cresce e si riduce automaticamente in base al numero di elementi memorizzati.
 *
 * ## Requisiti importanti
 * 1. L'utente deve specificare correttamente `sizeOfElements` in fase di creazione.
 * 2. Gli elementi devono poter essere confrontati tramite una funzione `cmp` fornita dall'utente.
 * 3. La tabella copia i byte dell'elemento: se il tuo tipo contiene puntatori interni,
 *    la copia è superficiale (shallow copy).
 *
 * ## Contratto della funzione cmp
 * La funzione di confronto deve restituire:
 * - `0` se i due valori sono uguali;
 * - `1` se il primo valore è maggiore del secondo;
 * - `-1` se il primo valore è minore del secondo;
 * - `-2` oppure `-3` in caso di puntatori nulli o errori analoghi.
 *
 * ### Esempio di cmp per int
 * @code
 * static int int_cmp(const void* a, const void* b) {
 *     if (a == NULL || b == NULL) return -2;
 *
 *     int x = *(const int*)a;
 *     int y = *(const int*)b;
 *
 *     if (x == y) return 0;
 *     if (x > y) return 1;
 *     return -1;
 * }
 * @endcode
 *
 * ## Esempio minimo di utilizzo
 * @code
 * #include <stdio.h>
 * #include "hash_table.h"
 *
 * static int int_cmp(const void* a, const void* b) {
 *     if (a == NULL || b == NULL) return -2;
 *
 *     int x = *(const int*)a;
 *     int y = *(const int*)b;
 *
 *     if (x == y) return 0;
 *     if (x > y) return 1;
 *     return -1;
 * }
 *
 * int main(void) {
 *     HashTable ht = NULL;
 *     int value = 42;
 *     bool found = false;
 *
 *     if (hashTable_create(&ht, 16, sizeof(int)) != OK) {
 *         return 1;
 *     }
 *
 *     if (hashTable_push(ht, int_cmp, &value) != OK) {
 *         hashTable_destroy(&ht);
 *         return 1;
 *     }
 *
 *     if (hashTable_search(ht, int_cmp, &value, &found) == OK && found) {
 *         printf("Valore trovato\n");
 *     }
 *
 *     hashTable_destroy(&ht);
 *     return 0;
 * }
 * @endcode
 */

#ifndef LIBFEM_HASH_TABLE_H
#define LIBFEM_HASH_TABLE_H

#include "third_party/xxHash.h"
#include <stdlib.h>

/** @brief Tipo booleano minimale usato dall'API. */
typedef char bool;
/** @brief Valore booleano vero. */
#define true 1
/** @brief Valore booleano falso. */
#define false 0

/**
 * @brief Codici di stato restituiti dalle funzioni dell'API.
 */
typedef enum {
  OK = 0,
  ERROR_NULL_POINTER = -1,
  ERROR_ALLOCATION_FAILURE = -2,
  ERROR_REALLOCATION_FAILURE = -3,
  ERROR_ARITHMETIC_OVERFLOW = -4,
  ERROR_WRONG_ELEMENTS_SIZE = -5,
  ERROR_SEARCHING_FOR_ZERO = -6,
  ERROR_VALUE_ALREADY_EXISTS = -7,
  ERROR_SEMANTIC_BUG_IT_SHOULDNT_HAPPEN = -8,
  ELEMENTO_NON_TROVATO = 1
} status_codes;

/**
 * @brief Fattore di crescita/riduzione della capacità.
 *
 * Deve essere strettamente maggiore di 1.
 */
#define HASH_TABLE_INCREMENT_MULTIPLIER 2

/** @brief Tipo opaco della hash table. */
typedef struct strHashTable* HashTable;

/**
 * @brief Crea una nuova hash table vuota.
 *
 * La tabella viene inizializzata con una capacità iniziale pari a `defaultCapacity`
 * e con elementi di dimensione fissa pari a `sizeOfElements` byte.
 *
 * @param[out] hashTable Puntatore alla variabile che riceverà la nuova hash table.
 * @param[in] defaultCapacity Capacità iniziale della tabella.
 * @param[in] sizeOfElements Dimensione, in byte, di ciascun elemento memorizzato.
 * @return `OK` in caso di successo, altrimenti un opportuno codice di errore.
 */
status_codes hashTable_create(HashTable* hashTable, size_t defaultCapacity, size_t sizeOfElements);

/**
 * @brief Distrugge la hash table e rilascia tutta la memoria associata.
 *
 * Dopo la distruzione, `*hashTable` viene impostato a `NULL`.
 *
 * @param[in,out] hashTable Puntatore alla hash table da distruggere.
 * @return `OK` in caso di successo, altrimenti un opportuno codice di errore.
 */
status_codes hashTable_destroy(HashTable* hashTable);

/**
 * @brief Calcola l'hash del valore passato usando XXH3_64bits.
 *
 * Questa funzione non inserisce il valore nella tabella: si limita a restituire
 * l'hash dei suoi byte.
 *
 * @param[in] hashTable Hash table da cui si ricava la dimensione dell'elemento.
 * @param[in] value Puntatore al valore di cui calcolare l'hash.
 * @param[out] result Variabile in cui scrivere l'hash calcolato.
 * @return `OK` in caso di successo, altrimenti un opportuno codice di errore.
 */
status_codes hashTable_makeHash(HashTable hashTable, void* value, unsigned long long int* result);

/**
 * @brief Inserisce un nuovo elemento nella hash table.
 *
 * In caso di collisione viene usato il linear probing.
 * Se l'elemento è già presente, la funzione restituisce `ERROR_VALUE_ALREADY_EXISTS`.
 *
 * @param[in] hashTable Hash table di destinazione.
 * @param[in] cmp Funzione di confronto tra due elementi.
 * @param[in] value Puntatore al valore da inserire.
 * @return `OK` in caso di successo, altrimenti un opportuno codice di errore.
 */
status_codes hashTable_push(HashTable hashTable, int (*cmp)(const void*, const void*), void* value);

/**
 * @brief Cerca un elemento nella hash table.
 *
 * La funzione scrive in `*result` se l'elemento è presente oppure no.
 *
 * @param[in] hashTable Hash table in cui effettuare la ricerca.
 * @param[in] cmp Funzione di confronto tra due elementi.
 * @param[in] value Puntatore al valore da cercare.
 * @param[out] result Variabile booleana che verrà impostata a `true` se trovato,
 *                    `false` altrimenti.
 * @return `OK` in caso di successo, altrimenti un opportuno codice di errore.
 */
status_codes hashTable_search(HashTable hashTable, int (*cmp)(const void*, const void*), void* value, bool* result);

/**
 * @brief Rimuove un elemento dalla hash table.
 *
 * La rimozione non libera fisicamente lo slot ma lo marca come `deleted`,
 * preservando la correttezza del probing.
 *
 * @param[in] hashTable Hash table da cui rimuovere l'elemento.
 * @param[in] cmp Funzione di confronto tra due elementi.
 * @param[in] value Puntatore al valore da rimuovere.
 * @return `OK` se l'elemento è stato rimosso, `ELEMENTO_NON_TROVATO` se non esiste,
 *         oppure un opportuno codice di errore.
 */
status_codes hashTable_remove(HashTable hashTable, int (*cmp)(const void*, const void*), void* value);

#endif // LIBFEM_HASH_TABLE_H
