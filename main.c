//
// Created by Spara on 20/04/2026.
//

#include "hash_table.h"

#include <stdio.h>
























int main() {
  HashTable hashTable;
  int testSize = 1000000;
  hashTable_create(&hashTable,testSize*2,sizeof(int));
  for (int i = 1; i < testSize; i++) {

    if (i%2 == 0) {
     status_codes res = hashTable_push(hashTable,&i);
      if (res != OK) printf("errore => %c\n",res);
    }
  }
  int val;
  do {

    printf("\ninserisci valore da cercare: ");
    scanf(" %d",&val);
    bool ris;
    status_codes res = hashTable_search(hashTable,&val,&ris);
    if (res != OK) printf("\nerrore => %c\n",res);
    else {
      if (ris) printf("\ntrovato");
      else printf("\nnon trovato");
    }



  }while (val != 0);

  return 0;
}