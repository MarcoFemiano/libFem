//
// Created by Spara on 15/04/2026.
//

#include "coda.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


struct strCoda {
  unsigned char* dati;
  size_t  size;
  size_t capacity;
  size_t sizeOfEachElement;
  size_t head;
  size_t tail;
  bool dinamica;
};

Coda coda_create(size_t capacity, size_t sizeOfEachElement) {

  //tests di robustezza
  if (sizeOfEachElement == 0 ) return NULL;
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
  if (coda->capacity > SIZE_MAX/sizeOfEachElement) {
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

      if (coda->capacity > SIZE_MAX / 2 / coda->sizeOfEachElement) return ERROR_REALLOC_FAIL;

      size_t oldCapacity = coda->capacity;
      unsigned char* datiTemp = malloc(coda->sizeOfEachElement * coda->capacity*2);
      if (datiTemp == NULL) return ERROR_REALLOC_FAIL;

      size_t headTemp = coda->head;

       for (size_t i = 0; i < coda->size; i++) {

        memcpy(
          datiTemp + (i * coda->sizeOfEachElement),
          coda->dati + (headTemp  * coda->sizeOfEachElement),
          coda->sizeOfEachElement);
         headTemp =  (headTemp +1) % oldCapacity;
       }

      free(coda->dati);
      coda->dati = datiTemp;
      coda->tail = coda->size;
      coda->head = 0;
      coda->capacity *=2;

    }



  }else {
    if (coda->size == coda->capacity) return ERROR_CODA_FULL;


  }

  memcpy((unsigned char*)coda->dati + (coda->sizeOfEachElement * coda->tail),datoInput,coda->sizeOfEachElement);
  coda->size++;
  coda->tail = (coda->tail + 1) % coda->capacity;
  return OK;


}

short coda_pop(Coda coda, void* datoOutput) {

  //tests di robustezza
  if (coda == NULL) return ERROR_NULL_POINTER;
  if (datoOutput == NULL) return ERROR_NULL_POINTER;
  if (coda->size == 0) return ERROR_CODA_EMPTY;

  memcpy(
    datoOutput,
    coda->dati + (coda->head  * coda->sizeOfEachElement),
    coda->sizeOfEachElement
    );

  coda->head = (coda->head + 1) % coda->capacity;
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
    coda->dati + (coda->head  * coda->sizeOfEachElement),
    coda->sizeOfEachElement
    );

  return OK;
}

long long int coda_size(Coda coda) {
  if (coda == NULL) return ERROR_NULL_POINTER;

  return (long long int)coda->size;

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