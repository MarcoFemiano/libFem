//
// Created by Spara on 15/04/2026.
//

#include "coda.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct strCoda {
  unsigned char* dati;
  long long int size;
  long long int capacity;
  size_t sizeOfEachElement;
  long long int head;
  long long int tail;
  char dinamica; //0 per statica, 1 per dinamica
};

Coda coda_create(long long int capacity, size_t sizeOfEachElement) {

  //tests di robustezza
  if (capacity < 0 || sizeOfEachElement == 0 ) return NULL;
  if (capacity == 0 && DEFAULT_SIZE == 0) return NULL;

  Coda coda = malloc(sizeof(struct strCoda));
  if (coda == NULL) return NULL;

  coda->dinamica = (char)(capacity == 0);

  if (coda->dinamica) coda->capacity = DEFAULT_SIZE;
  else coda->capacity = capacity;


  coda->size = 0;
  coda->head = 0;
  coda->tail = 0;
  coda->sizeOfEachElement = sizeOfEachElement;

  //test di robustezza
  if ((size_t)coda->capacity > SIZE_MAX/sizeOfEachElement) {
    free (coda);
    return NULL;
  }

  coda->dati = malloc(coda->capacity * sizeOfEachElement);

  //test di robustezza
  if (coda->dati == NULL) {
    free(coda);
    return NULL;

  }

  return coda;
}


short coda_destroy(Coda* coda) {

  //tests di robustezza
  if (coda == NULL) return ERROR_NULL_POINTER;
  if (*coda == NULL) return ERROR_NULL_POINTER;

  free((*coda)->dati);
  free(*coda);
  *coda = NULL;
  return OK;

}

short coda_push(Coda coda, const void* datoInput) {

  //tests di robustezza
  if (coda == NULL) return ERROR_NULL_POINTER;
  if (datoInput == NULL) return ERROR_NULL_POINTER;



  if (coda->dinamica) {

    if (coda->size == coda->capacity) {

      if ((size_t)coda->capacity > SIZE_MAX / 2 / coda->sizeOfEachElement) return ERROR_REALLOC_FAIL;



      long long int oldCapacity = coda->capacity;
      unsigned char* datiTemp = malloc(coda->sizeOfEachElement * coda->capacity*2);
      if (datiTemp == NULL) return ERROR_REALLOC_FAIL;
      coda->capacity *=2;



       for (long long int i = 0; i < coda->size; i++) {

        memcpy(
          (unsigned char*)datiTemp + (i * coda->sizeOfEachElement),
          (unsigned char*)coda->dati + (coda->head  * coda->sizeOfEachElement),
          coda->sizeOfEachElement);
         coda->head = (long long) (coda->head +1) % oldCapacity;
       }

      free(coda->dati);
      coda->dati = datiTemp;
      coda->tail = coda->size;
      coda->head = 0;

    }



  }else {
    if (coda->size == coda->capacity) return ERROR_CODA_FULL;


  }

  memcpy((unsigned char*)coda->dati + (coda->sizeOfEachElement * coda->tail),datoInput,coda->sizeOfEachElement);
  coda->size++;
  coda->tail = (long long)(coda->tail + 1) % coda->capacity;
  return OK;


}

short coda_pop(Coda coda, void* datoOutput) {

  //tests di robustezza
  if (coda == NULL) return ERROR_NULL_POINTER;
  if (datoOutput == NULL) return ERROR_NULL_POINTER;
  if (coda->size == 0) return ERROR_CODA_EMPTY;

  memcpy(
    datoOutput,
    (unsigned char*)coda->dati + (coda->head  * coda->sizeOfEachElement),
    coda->sizeOfEachElement
    );

  coda->head = (long long)(coda->head + 1) % coda->capacity;
  coda->size--;

  return OK;

}


short coda_peek(Coda coda, void* datoOutput) {
  //tests di robustezza
  if (coda == NULL) return ERROR_NULL_POINTER;
  if (datoOutput == NULL) return ERROR_NULL_POINTER;
  if (coda->size == 0) return ERROR_CODA_EMPTY;

  memcpy(
    datoOutput,
    (unsigned char*)coda->dati + (coda->head  * coda->sizeOfEachElement),
    coda->sizeOfEachElement
    );

  return OK;
}

long long int coda_size(Coda coda) {
  if (coda == NULL) return ERROR_NULL_POINTER;

  return coda->size;

}

short coda_is_empty(Coda coda) {
  if (coda == NULL) return ERROR_NULL_POINTER;

  return (short) (coda->size == 0);

}


short coda_is_full(Coda coda) {
  if (coda == NULL) return ERROR_NULL_POINTER;
  if (coda->dinamica) return ERROR_FUNCTION_NOT_VALID_IN_THIS_USE_CASE;

  return (short) (coda->size == coda->capacity);

}

short coda_clear(Coda coda) {
  if (coda == NULL) return ERROR_NULL_POINTER;

  coda->head = 0;
  coda->tail = 0;
  coda->size = 0;
  return OK;
}