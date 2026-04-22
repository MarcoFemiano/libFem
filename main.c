#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hash_table.h"

#define SMALL_CAPACITY 8
#define BIG_TEST_COUNT 1000000

static int int_cmp(const void* a, const void* b) {
    if (a == NULL || b == NULL) return -2;

    int x = *(const int*)a;
    int y = *(const int*)b;

    if (x == y) return 0;
    if (x < y) return -1;
    return 1;
}

static void print_test(const char* name, int passed) {
    printf("[%s] %s\n", passed ? "PASS" : "FAIL", name);
}

static double seconds_between(clock_t start, clock_t end) {
    return (double)(end - start) / (double)CLOCKS_PER_SEC;
}

int main(void) {
    HashTable ht = NULL;
    status_codes res;
    bool found = false;

    printf("========================================\n");
    printf("HASH TABLE TEST SUITE\n");
    printf("========================================\n\n");

    /* =========================================================
       TEST 1: CREATE
       ========================================================= */
    res = hashTable_create(&ht, SMALL_CAPACITY, sizeof(int));
    print_test("create", res == OK && ht != NULL);

    if (res != OK || ht == NULL) {
        printf("Impossibile continuare i test.\n");
        return 1;
    }

    /* =========================================================
       TEST 2: PUSH SEMPLICE
       ========================================================= */
    int a = 10;
    int b = 20;
    int c = 30;
    int d = 40;

    res = hashTable_push(ht, int_cmp, &a);
    print_test("push 10", res == OK);

    res = hashTable_push(ht, int_cmp, &b);
    print_test("push 20", res == OK);

    res = hashTable_push(ht, int_cmp, &c);
    print_test("push 30", res == OK);

    res = hashTable_push(ht, int_cmp, &d);
    print_test("push 40", res == OK);

    /* =========================================================
       TEST 3: SEARCH PRESENTI
       ========================================================= */
    found = false;
    res = hashTable_search(ht, int_cmp, &a, &found);
    print_test("search 10 presente", res == OK && found == true);

    found = false;
    res = hashTable_search(ht, int_cmp, &b, &found);
    print_test("search 20 presente", res == OK && found == true);

    found = false;
    res = hashTable_search(ht, int_cmp, &c, &found);
    print_test("search 30 presente", res == OK && found == true);

    found = false;
    res = hashTable_search(ht, int_cmp, &d, &found);
    print_test("search 40 presente", res == OK && found == true);

    /* =========================================================
       TEST 4: SEARCH ASSENTE
       ========================================================= */
    int absent = 999;
    found = false;
    res = hashTable_search(ht, int_cmp, &absent, &found);
    print_test("search assente", res == OK && found == false);

    /* =========================================================
       TEST 5: DUPLICATO
       ========================================================= */
    res = hashTable_push(ht, int_cmp, &a);
    print_test("push duplicato 10", res == ERROR_VALUE_ALREADY_EXISTS);

    /* =========================================================
       TEST 6: REMOVE
       ========================================================= */
    res = hashTable_remove(ht, int_cmp, &b);
    print_test("remove 20", res == OK);

    found = false;
    res = hashTable_search(ht, int_cmp, &b, &found);
    print_test("search 20 dopo remove", res == OK && found == false);

    /* =========================================================
       TEST 7: REMOVE ELEMENTO ASSENTE
       ========================================================= */
    res = hashTable_remove(ht, int_cmp, &absent);
    print_test("remove assente", res == ELEMENTO_NON_TROVATO);

    /* =========================================================
       TEST 8: REINSERT DOPO DELETE
       ========================================================= */
    res = hashTable_push(ht, int_cmp, &b);
    print_test("reinserimento 20 dopo delete", res == OK);

    found = false;
    res = hashTable_search(ht, int_cmp, &b, &found);
    print_test("search 20 dopo reinserimento", res == OK && found == true);

    /* =========================================================
       TEST 9: MOLTI INSERIMENTI + RESIZE
       ========================================================= */
    int ok_bulk = 1;
    for (int i = 100; i < 5000; i++) {
        res = hashTable_push(ht, int_cmp, &i);
        if (res != OK) {
            ok_bulk = 0;
            break;
        }
    }
    print_test("bulk insert fino a 4999", ok_bulk);

    int ok_bulk_search = 1;
    for (int i = 100; i < 5000; i++) {
        found = false;
        res = hashTable_search(ht, int_cmp, &i, &found);
        if (res != OK || found == false) {
            ok_bulk_search = 0;
            break;
        }
    }
    print_test("bulk search fino a 4999", ok_bulk_search);

    /* =========================================================
       TEST 10: REMOVE MASSIVO PARZIALE
       ========================================================= */
    int ok_bulk_remove = 1;
    for (int i = 100; i < 1000; i += 2) {
        res = hashTable_remove(ht, int_cmp, &i);
        if (res != OK) {
            ok_bulk_remove = 0;
            break;
        }
    }
    print_test("bulk remove pari da 100 a 998", ok_bulk_remove);

    int ok_post_remove = 1;
    for (int i = 100; i < 1000; i++) {
        found = false;
        res = hashTable_search(ht, int_cmp, &i, &found);
        if (res != OK) {
            ok_post_remove = 0;
            break;
        }

        if ((i % 2 == 0 && found != false) || (i % 2 != 0 && found != true)) {
            ok_post_remove = 0;
            break;
        }
    }
    print_test("search corretta dopo bulk remove", ok_post_remove);

    /* =========================================================
       DESTROY TABELLA PICCOLA
       ========================================================= */
    res = hashTable_destroy(&ht);
    print_test("destroy piccola", res == OK && ht == NULL);

    printf("\n========================================\n");
    printf("BENCHMARK 1.000.000 ELEMENTI\n");
    printf("========================================\n\n");

    /* =========================================================
       BENCHMARK GRANDE
       ========================================================= */
    HashTable big = NULL;
    res = hashTable_create(&big, 1024, sizeof(int));
    print_test("create grande", res == OK && big != NULL);

    if (res != OK || big == NULL) {
        printf("Impossibile eseguire benchmark.\n");
        return 1;
    }

    int* values = malloc(sizeof(int) * BIG_TEST_COUNT);
    if (values == NULL) {
        printf("[FAIL] allocazione array valori benchmark\n");
        hashTable_destroy(&big);
        return 1;
    }

    for (int i = 0; i < BIG_TEST_COUNT; i++) {
        values[i] = i;
    }

    clock_t insert_start = clock();
    for (int i = 0; i < BIG_TEST_COUNT; i++) {
        res = hashTable_push(big, int_cmp, &values[i]);
        if (res != OK) {
            printf("[FAIL] benchmark insert fallito all'indice %d con codice %d\n", i, res);
            free(values);
            hashTable_destroy(&big);
            return 1;
        }
    }
    clock_t insert_end = clock();

    clock_t search_start = clock();
    for (int i = 0; i < BIG_TEST_COUNT; i++) {
        found = false;
        res = hashTable_search(big, int_cmp, &values[i], &found);
        if (res != OK || found == false) {
            printf("[FAIL] benchmark search fallito all'indice %d con codice %d\n", i, res);
            free(values);
            hashTable_destroy(&big);
            return 1;
        }
    }
    clock_t search_end = clock();

    double insert_seconds = seconds_between(insert_start, insert_end);
    double search_seconds = seconds_between(search_start, search_end);

    printf("Inserimenti totali: %d\n", BIG_TEST_COUNT);
    printf("Tempo totale inserimenti: %.6f secondi\n", insert_seconds);
    printf("Ricerche totali: %d\n", BIG_TEST_COUNT);
    printf("Tempo totale ricerche: %.6f secondi\n", search_seconds);

    /* =========================================================
       CLEANUP
       ========================================================= */
    free(values);

    res = hashTable_destroy(&big);
    print_test("destroy grande", res == OK && big == NULL);

    printf("\n========================================\n");
    printf("FINE TEST\n");
    printf("========================================\n");

    return 0;
}