//
// Created by Spara on 20/04/2026.
//

#include "hash_table.h"

#include <stdio.h>

int main() {
  HashTable hashTable;
  hashTable_create(&hashTable,10,sizeof(int));
  int e = 8;
  unsigned long long int hash;
  hashTable_makeHash(hashTable,&e,&hash);
  printf("%llu\n",hash);
  return 0;
}