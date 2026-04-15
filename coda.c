//
// Created by Spara on 15/04/2026.
//

#include "coda.h"

#include <stddef.h>
#include <stdlib.h>

struct strCoda {
  void* dati;
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

  //Se capacity è 0 dinamica diventerà 1. Altrimenti sarà 0
  coda->dinamica =(char) !(capacity);

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


void coda_destroy(Coda* coda) {



}

short coda_push(Coda coda, const void* datoInput) {

}

short coda_pop(Coda coda, void* datoOutput) {

}


short coda_peek(Coda coda, void* datoOutput) {

}

long long int coda_size(Coda coda) {

}

short coda_is_empty(Coda coda) {

}


short coda_is_full(Coda coda) {

}