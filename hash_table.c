//
// Created by Spara on 20/04/2026.
//

#include "hash_table.h"

#include <stddef.h>

struct strHashTable {
  void* data;
  size_t size;
  size_t capacity;
  size_t defaultCapacity;
  size_t sizeOfelements;
};

status_codes hashTable_create(HashTable* hashTable,size_t defaultCapacity,size_t sizeOfElements) {
  //tests di robustezza
  if (hashTable == NULL) return ERROR_NULL_POINTER;
  if (sizeOfElements == 0) return ERROR_WRONG_ELEMENTS_SIZE;

  HashTable newHashTable = malloc (sizeof(struct strHashTable));
  if (newHashTable == NULL) return ERROR_ALLOCATION_FAILURE;

  newHashTable->size = 0;
  newHashTable->capacity = defaultCapacity;
  newHashTable->defaultCapacity = defaultCapacity;
  newHashTable->sizeOfelements = sizeOfElements;
  if (sizeOfElements > SIZE_MAX / newHashTable->capacity) return ERROR_ARITHMETIC_OVERFLOW;

  newHashTable->data = malloc (newHashTable->capacity * sizeOfElements);
  if (newHashTable->data == NULL) {
    free(newHashTable);
    return ERROR_ALLOCATION_FAILURE;
  }

  *hashTable = newHashTable;

  return OK;
}

status_codes hashTable_destroy(HashTable* hashTable) {
  //tests di robustezza
  if (hashTable == NULL || *hashTable == NULL || (*hashTable)->data) return ERROR_NULL_POINTER;

  free((*hashTable)->data);
  free(*hashTable);
  *hashTable = NULL;

  return OK;
}

status_codes hashTable_makeHash(HashTable hashTable, void* value,unsigned long long int* result) {
  if (hashTable == NULL || value == NULL || result == NULL) return ERROR_NULL_POINTER;

  *result = XXH3_64bits(value, hashTable->sizeOfelements);
  return OK;
}