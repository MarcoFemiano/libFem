#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hash_table.h"

int int_cmp(const void *a, const void *b) {
      if (a == NULL | b == NULL) return -2;

         const int *x = a;
         const int *y = b;

         if (*x < *y) return -1;
        if (*x > *y) return 1;
        return 0;
     }

int main(void) {
    HashTable ht = NULL;
    status_codes res;
    bool found = false;
    hashTable_create(&ht,10,sizeof(int));

    int quanti = 10000;
    for (int i = 0; i<quanti; i++) {
        hashTable_push(ht,int_cmp,&i);
    }

    int scelta;
    do {
        printf("inserisci valore da cercare: ");
        scanf(" %d",&scelta);
        res = hashTable_search(ht,int_cmp,&scelta,&found);
         if (res != OK) return res;
        if (found) printf("\nTrovato\n");
        else printf("\nNON Trovato\n");

    }while (scelta != 0);


    do {
        printf("inserisci valore da rimuovere: ");
        scanf(" %d",&scelta);
        res = hashTable_remove(ht,int_cmp,&scelta);
        if (res != OK) return res;
        printf("\nRimosso\n");

    }while (scelta != 0);

    do {
        printf("inserisci valore da cercare: ");
        scanf(" %d",&scelta);
        res = hashTable_search(ht,int_cmp,&scelta,&found);
        if (res != OK) return res;
        if (found) printf("\nTrovato\n");
        else printf("\nNON Trovato\n");

    }while (scelta != 0);

    return 0;
}