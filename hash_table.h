//
// Created by Spara on 20/04/2026.
//

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
  ERROR_SEARCHING_FOR_ZERO = -6
}status_codes;

#define HASH_TABLE_INCREMENT_MULTIPLIER 2 //MUST BE >1

typedef struct strHashTable* HashTable;

status_codes hashTable_create(HashTable* hashTable,size_t defaultCapacity,size_t sizeOfElements);

status_codes hashTable_destroy(HashTable* hashTable);

status_codes hashTable_makeHash(HashTable hashTable, void* value,unsigned long long int* result);

status_codes hashTable_push(HashTable hashTable, void* value);

status_codes hashTable_search(HashTable hashTable, void* value, bool* result);

#endif // LIBFEM_HASH_TABLE_H