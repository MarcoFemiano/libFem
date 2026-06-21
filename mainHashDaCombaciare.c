#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
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
int random_non_zero(void) {
    int v;
    do {
        v = rand();
    } while (v == 0);
    return v;
}

#define HASH_DA_COMBACIARE 3
#define STEPS_BEFORE_WAIT 100000
int main(void) {

    srand((unsigned int) time(NULL));
    int x[HASH_DA_COMBACIARE+1 ];
    for (int i = 0; i <= HASH_DA_COMBACIARE; i++) x[i] = random_non_zero();
    HashTable ht = NULL;
    status_codes res;
    bool found = false;
    hashTable_create(&ht,10,sizeof(int));

    unsigned long long int resHash;
    int i = 0;
    int correct = 0;
    unsigned long long int steps = 0;

    while (1) {

            steps++;
        for (int i2 = 0; i2 < HASH_DA_COMBACIARE; i2++) {
            hashTable_makeHash(ht,&i,&resHash);
            x[i2+1] = random_non_zero();
            if ((resHash%x[i2]) == 0) {
                correct++;
                if (correct == HASH_DA_COMBACIARE) goto victory;
            }else {
                i++;
                correct = 0;
                break;
            }
        }


        if (steps % STEPS_BEFORE_WAIT == 0)
        Sleep(1);

    }

    victory:
    printf("done in %llu steps\n",steps);



    return 0;
}