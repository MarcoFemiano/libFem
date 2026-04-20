//
// Created by Spara on 20/04/2026.
//

#include "hash_table.h"

#include <stddef.h>

struct strHashTable {
  unsigned long long int* data;
  size_t size;
  size_t capacity;
  size_t defaultCapacity;
  size_t sizeOfelements;
};

status_codes hashTable_create(HashTable* hashTable,size_t defaultCapacity,size_t sizeOfElements) {
  //tests di robustezza
  if (hashTable == NULL) return ERROR_NULL_POINTER;
  if (sizeOfElements == 0) return ERROR_WRONG_ELEMENTS_SIZE;
  if (defaultCapacity == 0) return ERROR_ALLOCATION_FAILURE;

  HashTable newHashTable = malloc (sizeof(struct strHashTable));
  if (newHashTable == NULL) return ERROR_ALLOCATION_FAILURE;

  newHashTable->size = 0;
  newHashTable->capacity = defaultCapacity;
  newHashTable->defaultCapacity = defaultCapacity;
  newHashTable->sizeOfelements = sizeOfElements;

  if (defaultCapacity > SIZE_MAX / sizeof(unsigned long long int))

  newHashTable->data = malloc (newHashTable->capacity * sizeof(unsigned long long int));

  if (newHashTable->data == NULL) {
    free(newHashTable);
    return ERROR_ALLOCATION_FAILURE;
  }

  *hashTable = newHashTable;

  return OK;
}

status_codes hashTable_destroy(HashTable* hashTable) {
  //tests di robustezza
  if (hashTable == NULL || *hashTable == NULL || (*hashTable)->data == NULL) return ERROR_NULL_POINTER;

  free((*hashTable)->data);
  free(*hashTable);
  *hashTable = NULL;

  return OK;
}


static status_codes hashTable_makeHash(HashTable hashTable, void* value,unsigned long long int* result) {
  if (hashTable == NULL || value == NULL || result == NULL) return ERROR_NULL_POINTER;

  *result = XXH3_64bits(value, hashTable->sizeOfelements);
  return OK;
}


//non va bene. da rifare
static status_codes hashTable_rehash(HashTable hashTable) {
  if (hashTable == NULL || hashTable->data == NULL) return ERROR_NULL_POINTER;

  for (size_t i = 0; i < hashTable->size; i++) {
      unsigned long long int hash = XXH3_64bits(&hashTable->data[i],sizeof(unsigned long long int));
      hash = hash % hashTable->capacity;
      status_codes res = hashTable_push(hashTable, &hash);
      if (res != OK) return res;
  }

  return OK;
}

static status_codes hashTable_adjustCapacity(HashTable hashTable) {
  if (hashTable == NULL || hashTable->data == NULL) return ERROR_NULL_POINTER;

  size_t newCapacity = 0;
  //espansione
  if (hashTable->size == hashTable->capacity) {
    newCapacity = hashTable->capacity * HASH_TABLE_INCREMENT_MULTIPLIER;
    if (newCapacity  > SIZE_MAX / hashTable->capacity ) return ERROR_ARITHMETIC_OVERFLOW;

    void* temp = realloc(hashTable->data,newCapacity*sizeof(unsigned long long int));
    if (temp == NULL) return ERROR_ALLOCATION_FAILURE;

    hashTable->data = temp;
  }//riduzione
  else if (hashTable->size < hashTable->capacity/HASH_TABLE_INCREMENT_MULTIPLIER) {

    newCapacity = hashTable->capacity / HASH_TABLE_INCREMENT_MULTIPLIER;
    void* temp = realloc(hashTable->data,newCapacity*sizeof(unsigned long long int));
    if (temp == NULL) return ERROR_ALLOCATION_FAILURE;
    hashTable->data = temp;


  }

  hashTable->capacity = newCapacity;
  return OK;
}


status_codes hashTable_push(HashTable hashTable, void* value) {
  if (hashTable == NULL || value == NULL) return ERROR_NULL_POINTER;

  unsigned long long int hash = XXH3_64bits(value,hashTable->sizeOfelements);

  status_codes res = hashTable_adjustCapacity(hashTable);
  if (res != OK) return res;

  hash = hash % hashTable->capacity;
  hashTable->data[hashTable->size] = hash;
  hashTable->size++;
  return OK;
}