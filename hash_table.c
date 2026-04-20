//
// Created by Spara on 20/04/2026.
//

#include "hash_table.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

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
  if (defaultCapacity == 0) return ERROR_ALLOCATION_FAILURE;

  HashTable newHashTable = malloc (sizeof(struct strHashTable));
  if (newHashTable == NULL) return ERROR_ALLOCATION_FAILURE;

  newHashTable->size = 0;
  newHashTable->capacity = defaultCapacity;
  newHashTable->defaultCapacity = defaultCapacity;
  newHashTable->sizeOfelements = sizeOfElements;

  if (defaultCapacity > SIZE_MAX / sizeOfElements) {
    free(newHashTable);
    return ERROR_ARITHMETIC_OVERFLOW;
  }

  newHashTable->data = calloc (newHashTable->capacity, sizeOfElements);

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


 status_codes hashTable_makeHash(HashTable hashTable, void* value,unsigned long long int* result) {
  if (hashTable == NULL || value == NULL || result == NULL) return ERROR_NULL_POINTER;

  *result = XXH3_64bits(value, hashTable->sizeOfelements);
  return OK;
}

static status_codes hashTable_pushWithoutResize(HashTable hashTable, void* value) {
  if (hashTable == NULL || value == NULL || hashTable->data == NULL) return ERROR_NULL_POINTER;

  unsigned long long int hash = XXH3_64bits(value,hashTable->sizeOfelements);

  hash = hash % hashTable->capacity;
  //hashTable->data[hashTable->size] = hash;

  memcpy(((unsigned char*)hashTable->data+(hash*hashTable->sizeOfelements)),value,hashTable->sizeOfelements);
  hashTable->size++;
  return OK;
}

static status_codes hashTable_rehash(HashTable hashTable) {
  if (hashTable == NULL || hashTable->data == NULL) return ERROR_NULL_POINTER;

  void* temp = calloc(hashTable->sizeOfelements,hashTable->capacity);
  if (temp == NULL) return ERROR_ALLOCATION_FAILURE;

  void* valueZero = calloc(1, hashTable->sizeOfelements);
  if (valueZero == NULL) {
    free(temp);
    return ERROR_ALLOCATION_FAILURE;
  }
  unsigned long long int zeroHash = XXH3_64bits(valueZero, hashTable->sizeOfelements);

  unsigned long long int tempHash;
  for (size_t i = 0; i < hashTable->capacity; i++) {
    //copio gli elementi in un nuovo array che diventerà la nuova hash table
    //SE SONO DIVERSI DA 0
    tempHash = XXH3_64bits(((unsigned char*)hashTable->data + i*hashTable->sizeOfelements), hashTable->sizeOfelements);
    if (tempHash == zeroHash) continue;
    memcpy((unsigned char*)temp + i * hashTable->sizeOfelements ,((unsigned char*)hashTable->data + i*hashTable->sizeOfelements),hashTable->sizeOfelements);
  }
    free(hashTable->data);
    hashTable->data = calloc(hashTable->capacity,hashTable->sizeOfelements);
    if (hashTable->data == NULL) {
      free(valueZero);
      free(temp);
    return ERROR_ALLOCATION_FAILURE;
  }

  //inserisco i nuovi hash
  status_codes res2;
  hashTable->size = 0;
  for (size_t i = 0; i < hashTable->capacity; i++) {

    tempHash = XXH3_64bits(((unsigned char*)temp + i*hashTable->sizeOfelements), hashTable->sizeOfelements);
    if (tempHash == zeroHash) continue;

    res2 = hashTable_pushWithoutResize(hashTable,(unsigned char*)temp + i * hashTable->sizeOfelements);

    if (res2 != OK) {
      free(valueZero);
      free(temp);
      return res2;
    }
  }
  free(temp);
  free(valueZero);
  return OK;
}

static status_codes hashTable_adjustCapacity(HashTable hashTable) {
  if (hashTable == NULL || hashTable->data == NULL) return ERROR_NULL_POINTER;

  size_t newCapacity = hashTable->capacity;
  //espansione
  if (hashTable->size == hashTable->capacity) {
    newCapacity = hashTable->capacity * HASH_TABLE_INCREMENT_MULTIPLIER;
    if (newCapacity > SIZE_MAX / hashTable->sizeOfelements) return ERROR_ARITHMETIC_OVERFLOW;

    void* temp = realloc(hashTable->data,newCapacity*hashTable->sizeOfelements);
    if (temp == NULL) return ERROR_ALLOCATION_FAILURE;

    hashTable->data = temp;
    hashTable->capacity = newCapacity;

    status_codes res = hashTable_rehash(hashTable);
    if (res != OK) return res;
  }//riduzione
  else if (hashTable->size < hashTable->capacity/HASH_TABLE_INCREMENT_MULTIPLIER ) {

    if (newCapacity < hashTable->defaultCapacity) {
      newCapacity = hashTable->defaultCapacity;
    }else {
      newCapacity = hashTable->capacity / HASH_TABLE_INCREMENT_MULTIPLIER;
      void* temp = realloc(hashTable->data,newCapacity*hashTable->sizeOfelements);
      if (temp == NULL) return ERROR_ALLOCATION_FAILURE;
      hashTable->data = temp;
      hashTable->capacity = newCapacity;

      status_codes res = hashTable_rehash(hashTable);
      if (res != OK) return res;
    }
  }


  return OK;
}


status_codes hashTable_push(HashTable hashTable, void* value) {
  if (hashTable == NULL || value == NULL) return ERROR_NULL_POINTER;

  status_codes res = hashTable_adjustCapacity(hashTable);
  if (res != OK) return res;

  return hashTable_pushWithoutResize(hashTable, value);
}


status_codes hashTable_search(HashTable hashTable, void* value, bool *result) {
  if (hashTable == NULL || value == NULL || result == NULL) {
    return ERROR_NULL_POINTER;
  }

  void* zero = calloc(1, hashTable->sizeOfelements);
  if (zero == NULL) {
    return ERROR_ALLOCATION_FAILURE;
  }

  *result = false;

  unsigned long long int hashZero = XXH3_64bits(zero, hashTable->sizeOfelements);
  unsigned long long int hashToSeek = XXH3_64bits(value, hashTable->sizeOfelements);

  if (hashToSeek == hashZero) {
    free(zero);
    return ERROR_SEARCHING_FOR_ZERO;
  }

  size_t index = hashToSeek % hashTable->capacity;
  void* slot = (unsigned char*)hashTable->data + index * hashTable->sizeOfelements;

  if (memcmp(slot, zero, hashTable->sizeOfelements) != 0) {
    *result = true;
  }

  free(zero);
  return OK;
}