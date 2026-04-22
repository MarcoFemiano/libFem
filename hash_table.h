//
// Created by Spara on 20/04/2026.
//

/*
 * NELLA FUNZIONE cmp L'UTENTE DEVE RESTITUIRE:
 * 1 => a > b;
 * -1 => b > a;
 * 0 => a == b
 * -2 e -3 per puntatori nulli
 *
 */
#ifndef LIBFEM_HASH_TABLE_H
#define LIBFEM_HASH_TABLE_H
#include "third_party/xxHash.h"

#include <stdlib.h>

typedef char bool;
#define true 1
#define false 0
typedef enum {
  OK = 0,
  ERROR_NULL_POINTER = -1,
  ERROR_ALLOCATION_FAILURE = -2,
  ERROR_REALLOCATION_FAILURE =-3,
  ERROR_ARITHMETIC_OVERFLOW = -4,
  ERROR_WRONG_ELEMENTS_SIZE = -5,
  ERROR_SEARCHING_FOR_ZERO = -6,
  ERROR_VALUE_ALREADY_EXISTS = -7,
    ERROR_SEMANTIC_BUG_IT_SHOULDNT_HAPPEN = -8,
    ELEMENTO_NON_TROVATO = 1
}status_codes;

#define HASH_TABLE_INCREMENT_MULTIPLIER 2 //MUST BE >1

typedef struct strHashTable* HashTable;

status_codes hashTable_create(HashTable* hashTable,size_t defaultCapacity,size_t sizeOfElements);

status_codes hashTable_destroy(HashTable* hashTable);

status_codes hashTable_makeHash(HashTable hashTable, void* value,unsigned long long int* result);

status_codes hashTable_push(HashTable hashTable,int (*cmp)(const void*, const void*), void* value);

status_codes hashTable_search(HashTable hashTable,int (*cmp)(const void*, const void*) ,void* value, bool* result);

status_codes hashTable_remove(HashTable hashTable,int (*cmp)(const void*, const void*) ,void* value);

#endif // LIBFEM_HASH_TABLE_H