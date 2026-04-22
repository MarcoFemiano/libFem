//
// Created by Spara on 20/04/2026.
//

#include "hash_table.h"


#include <string.h>

struct strHashTable {
  void* data;
  char* slotStates; //2=> deleted ; 1 => occupied   ; 0 => free
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



  newHashTable->slotStates = calloc (newHashTable->capacity, sizeof(char));
  if (newHashTable->slotStates == NULL) {
    free(newHashTable->data);
    free(newHashTable);
    return ERROR_ALLOCATION_FAILURE;
  }

  *hashTable = newHashTable;

  return OK;
}

status_codes hashTable_destroy(HashTable* hashTable) {
  //tests di robustezza
  if (hashTable == NULL || *hashTable == NULL || (*hashTable)->data == NULL || (*hashTable)->slotStates == NULL) return ERROR_NULL_POINTER;

  free((*hashTable)->data);
  free((*hashTable)->slotStates);
  free(*hashTable);
  *hashTable = NULL;

  return OK;
}


 status_codes hashTable_makeHash(HashTable hashTable, void* value,unsigned long long int* result) {
  if (hashTable == NULL || value == NULL || result == NULL) return ERROR_NULL_POINTER;

  *result = XXH3_64bits(value, hashTable->sizeOfelements);
  return OK;
}



static status_codes hashTable_pushWithoutResize(HashTable hashTable, int (*cmp)(const void*, const void*), void* value) {
  if (hashTable == NULL || hashTable->data == NULL || hashTable->slotStates == NULL || cmp == NULL || value == NULL) {
    return ERROR_NULL_POINTER;
  }

  size_t tentativi = 0;
  size_t firstDeleted = hashTable->capacity; // valore sentinella
  unsigned long long int hashValue = XXH3_64bits(value, hashTable->sizeOfelements) % hashTable->capacity;
  void* slot;

  LINEAR_PROBING:
    if (tentativi == hashTable->capacity) {
      if (firstDeleted != hashTable->capacity) {
        void* deletedSlot = (unsigned char*)hashTable->data + firstDeleted * hashTable->sizeOfelements;
        memcpy(deletedSlot, value, hashTable->sizeOfelements);
        hashTable->slotStates[firstDeleted] = 1;
        hashTable->size++;
        return OK;
      }

      return ERROR_SEMANTIC_BUG_IT_SHOULDNT_HAPPEN;
    }

  slot = (unsigned char*)hashTable->data + hashValue * hashTable->sizeOfelements;

  if (hashTable->slotStates[hashValue] == 0) {
    size_t targetIndex = (firstDeleted != hashTable->capacity) ? firstDeleted : hashValue;
    void* targetSlot = (unsigned char*)hashTable->data + targetIndex * hashTable->sizeOfelements;

    memcpy(targetSlot, value, hashTable->sizeOfelements);
    hashTable->slotStates[targetIndex] = 1;
    hashTable->size++;
    return OK;
  }

  if (hashTable->slotStates[hashValue] == 2) {
    if (firstDeleted == hashTable->capacity) firstDeleted = hashValue;

    hashValue = (hashValue + 1) % hashTable->capacity;
    tentativi++;
    goto LINEAR_PROBING;
  }

  int res = cmp(slot, value);
  if (res == 0) return ERROR_VALUE_ALREADY_EXISTS;
  if (res < -1) return ERROR_NULL_POINTER;

  hashValue = (hashValue + 1) % hashTable->capacity;
  tentativi++;
  goto LINEAR_PROBING;
}
/*
static status_codes hashTable_rehash(HashTable hashTable, int (*cmp)(const void*, const void*)) {
  if (hashTable == NULL || hashTable->data == NULL || hashTable->slotStates == NULL || cmp == NULL) return ERROR_NULL_POINTER;


  void* oldData = hashTable->data;
  char* oldStates = hashTable->slotStates;
  size_t oldCapacity = hashTable->capacity;
  size_t oldSize = hashTable->size;

  void* newData = calloc(hashTable->capacity, hashTable->sizeOfelements);
  if (newData == NULL) return ERROR_ALLOCATION_FAILURE;

  char* newStates = calloc(hashTable->capacity, sizeof(char));
  if (newStates == NULL) {
    free(newData);
    return ERROR_ALLOCATION_FAILURE;
  }

  hashTable->data = newData;
  hashTable->slotStates = newStates;
  hashTable->size = 0;

  for (size_t i = 0; i < oldCapacity; i++) {
    if (oldStates[i] == 0) continue;

    status_codes res = hashTable_pushWithoutResize(
        hashTable,
        cmp,
        (unsigned char*)oldData + i * hashTable->sizeOfelements
    );

    if (res != OK) {
      free(newData);
      free(newStates);

      hashTable->data = oldData;
      hashTable->slotStates = oldStates;
      hashTable->capacity = oldCapacity;
      hashTable->size = oldSize;

      return res;
    }
  }

  free(oldData);
  free(oldStates);
  return OK;
}*/

static status_codes hashTable_adjustCapacity(HashTable hashTable, int (*cmp)(const void*,const void*)) {
  if (hashTable == NULL || hashTable->data == NULL || hashTable->slotStates == NULL || cmp == NULL) return ERROR_NULL_POINTER;

  size_t newCapacity = hashTable->capacity;
  // espansione
  if (hashTable->size == hashTable->capacity) {
    newCapacity = hashTable->capacity * HASH_TABLE_INCREMENT_MULTIPLIER;
    if (newCapacity > SIZE_MAX / hashTable->sizeOfelements) {
      return ERROR_ARITHMETIC_OVERFLOW;
    }

    // salvo i vecchi buffer
    void* oldData = hashTable->data;
    char* oldStates = hashTable->slotStates;
    size_t oldCapacity = hashTable->capacity;
    size_t oldSize = hashTable->size;

    // alloco i nuovi buffer più grandi
    void* newData = calloc(newCapacity, hashTable->sizeOfelements);
    if (newData == NULL) return ERROR_ALLOCATION_FAILURE;

    char* newSlotStates = calloc(newCapacity, sizeof(char));
    if (newSlotStates == NULL) {
      free(newData);
      return ERROR_ALLOCATION_FAILURE;
    }

    // faccio puntare temporaneamente la hash table ai nuovi buffer
    hashTable->data = newData;
    hashTable->slotStates = newSlotStates;
    hashTable->capacity = newCapacity;
    hashTable->size = 0;

    // migro gli elementi vecchi nei nuovi buffer
    for (size_t i = 0; i < oldCapacity; i++) {
      if (oldStates[i] != 1) continue;

      status_codes res2 = hashTable_pushWithoutResize(
          hashTable,
          cmp,
          (unsigned char*)oldData + i * hashTable->sizeOfelements
      );

      if (res2 != OK) {
        // rollback
        free(newData);
        free(newSlotStates);

        hashTable->data = oldData;
        hashTable->slotStates = oldStates;
        hashTable->capacity = oldCapacity;
        hashTable->size = oldSize;

        return res2;
      }
    }

    // se tutto è andato bene, libero i vecchi buffer
    free(oldData);
    free(oldStates);
  }// riduzione
  else if (hashTable->size < hashTable->capacity / HASH_TABLE_INCREMENT_MULTIPLIER) {

    newCapacity = hashTable->capacity / HASH_TABLE_INCREMENT_MULTIPLIER;
    if (newCapacity < hashTable->defaultCapacity) {
      newCapacity = hashTable->defaultCapacity;
    }

    // se non cambia davvero nulla, esci
    if (newCapacity == hashTable->capacity) {
      return OK;
    }

    // salvo i vecchi buffer
    void* oldData = hashTable->data;
    char* oldSlotStates = hashTable->slotStates;
    size_t oldCapacity = hashTable->capacity;
    size_t oldSize = hashTable->size;

    // alloco i nuovi buffer più piccoli
    void* newData = calloc(newCapacity, hashTable->sizeOfelements);
    if (newData == NULL) return ERROR_ALLOCATION_FAILURE;

    char* newSlotStates = calloc(newCapacity, sizeof(char));
    if (newSlotStates == NULL) {
      free(newData);
      return ERROR_ALLOCATION_FAILURE;
    }

    // faccio puntare temporaneamente la hash table ai nuovi buffer
    hashTable->data = newData;
    hashTable->slotStates = newSlotStates;
    hashTable->capacity = newCapacity;
    hashTable->size = 0;

    // migro gli elementi vecchi nei nuovi buffer
    for (size_t i = 0; i < oldCapacity; i++) {
      if (oldSlotStates[i] != 1) continue;

      status_codes res2 = hashTable_pushWithoutResize(hashTable,cmp,(unsigned char*)oldData + i * hashTable->sizeOfelements);

      if (res2 != OK) {
        // rollback
        free(newData);
        free(newSlotStates);

        hashTable->data = oldData;
        hashTable->slotStates = oldSlotStates;
        hashTable->capacity = oldCapacity;
        hashTable->size = oldSize;

        return res2;
      }
    }

    // se tutto è andato bene, libero i vecchi buffer
    free(oldData);
    free(oldSlotStates);
  }


  return OK;
}


status_codes hashTable_push(HashTable hashTable,int (*cmp)(const void*, const void*) ,void* value) {
  if (hashTable == NULL || value == NULL) return ERROR_NULL_POINTER;

  status_codes res = hashTable_adjustCapacity(hashTable,cmp);
  if (res != OK) return res;

  return hashTable_pushWithoutResize(hashTable,cmp ,value);
}


status_codes hashTable_search(HashTable hashTable,int (*cmp)(const void*, const void*) ,void* value, bool *result) {
  if (hashTable == NULL || value == NULL || hashTable->slotStates == NULL || hashTable->data == NULL || result == NULL || cmp == NULL) {
    return ERROR_NULL_POINTER;
  }

  *result = false;
  unsigned long long int hashToSeek = XXH3_64bits(value, hashTable->sizeOfelements);

  size_t index = hashToSeek % hashTable->capacity;
  void* slotElement;

  for (size_t tentativi = 0; tentativi < hashTable->capacity; tentativi++) {
    // se trovo uno slot mai usato, l'elemento non esiste
    if (hashTable->slotStates[index] == 0) return OK;

    // se è deleted, devo continuare a cercare
    if (hashTable->slotStates[index] == 2) {
      index = (index + 1) % hashTable->capacity;
      continue;
    }

    slotElement = (unsigned char*)hashTable->data + index * hashTable->sizeOfelements;

    int res = cmp(slotElement, value);
    // se c'è un errore esco
    if (res < -1) return res;

    if (res == 0) {
      *result = true;
      return OK;
    }

    index = (index + 1) % hashTable->capacity;
  }

  return OK;
}



status_codes hashTable_remove(HashTable hashTable,int (*cmp)(const void*, const void*) ,void* value) {
  if (hashTable == NULL || hashTable->slotStates == NULL || hashTable->data == NULL || cmp == NULL || value == NULL) {
    return ERROR_NULL_POINTER;
  }

  status_codes res;
  size_t tentativi = 0;
  unsigned long long int index = XXH3_64bits(value, hashTable->sizeOfelements);
  index = index % hashTable->capacity;
  void* slotElement;

  LINEAR_PROBING:

  if (tentativi == hashTable->capacity) return ELEMENTO_NON_TROVATO;

  if (hashTable->slotStates[index] == 0) return ELEMENTO_NON_TROVATO;

  if (hashTable->slotStates[index] == 2) {
    index = (index + 1) % hashTable->capacity;
    tentativi++;
    goto LINEAR_PROBING;
  }

  slotElement = (unsigned char*)hashTable->data + index * hashTable->sizeOfelements;
  res = cmp(slotElement, value);

  if (res < -1) return res;

  if (res == 0) {
    hashTable->slotStates[index] = 2;
    hashTable->size--;
    return OK;
  }

  index = (index + 1) % hashTable->capacity;
  tentativi++;
  goto LINEAR_PROBING;
}
