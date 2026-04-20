#include <stdio.h>
#include <stdlib.h>

#include "BST_AVL.h"

#define MAX_TRACKED_VALUES 4096

static int* trackedValues[MAX_TRACKED_VALUES];
static size_t trackedCount = 0;

static int testsPassed = 0;
static int testsFailed = 0;

static int cmp_int(const void* a, const void* b) {
        int valA = *(const int*)a;
        int valB = *(const int*)b;

        if (valA > valB) return 1;
        if (valA < valB) return -1;
        return 0;
}

static void print_int(const void* value) {
        if (value == NULL) {
                printf("NULL");
                return;
        }

        printf("%d", *(const int*)value);
}

static const char* status_to_string(status_codes code) {
        switch (code) {
                case OK: return "OK";
                case ERROR_NULL_POINTER: return "ERROR_NULL_POINTER";
                case ERROR_ARITHMETIC_OVERFLOW: return "ERROR_ARITHMETIC_OVERFLOW";
                case ERROR_REALLOC_FAILURE: return "ERROR_REALLOC_FAILURE";
                case ERROR_ALLOCATION_FAILURE: return "ERROR_ALLOCATION_FAILURE";
                case ERROR_EMPTY_TREE: return "ERROR_EMPTY_TREE";
                case ERROR_NOT_FOUND: return "ERROR_NOT_FOUND";
                case ERROR_INVALID_ARGUMENT: return "ERROR_INVALID_ARGUMENT";
                case NODE_ALREADY_EXISTS: return "NODE_ALREADY_EXISTS";
                default: return "UNKNOWN_STATUS_CODE";
        }
}

static void clear_input_buffer(void) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
}

static int* alloc_int(int value) {
        int* ptr = malloc(sizeof(int));
        if (ptr == NULL) return NULL;
        *ptr = value;
        return ptr;
}

static void track_ptr(int* ptr) {
        if (ptr == NULL) return;
        if (trackedCount < MAX_TRACKED_VALUES) {
                trackedValues[trackedCount++] = ptr;
        }
}

static void free_all_tracked_values(void) {
        size_t i;
        for (i = 0; i < trackedCount; i++) {
                free(trackedValues[i]);
                trackedValues[i] = NULL;
        }
        trackedCount = 0;
}

static void begin_test(const char* name) {
        printf("\n[TEST] %s\n", name);
}

static void pass_test(const char* message) {
        printf("  [OK] %s\n", message);
        testsPassed++;
}

static void fail_test(const char* message) {
        printf("  [FAIL] %s\n", message);
        testsFailed++;
}

static void expect_status(const char* label, status_codes actual, status_codes expected) {
        if (actual == expected) {
                pass_test(label);
        } else {
                char buffer[256];
                snprintf(buffer, sizeof(buffer),
                         "%s -> atteso %s, ottenuto %s",
                         label,
                         status_to_string(expected),
                         status_to_string(actual));
                fail_test(buffer);
        }
}

static void expect_bool(const char* label, bool actual, bool expected) {
        if (actual == expected) {
                pass_test(label);
        } else {
                char buffer[256];
                snprintf(buffer, sizeof(buffer),
                         "%s -> atteso %d, ottenuto %d",
                         label,
                         (int)expected,
                         (int)actual);
                fail_test(buffer);
        }
}

static void expect_int(const char* label, int actual, int expected) {
        if (actual == expected) {
                pass_test(label);
        } else {
                char buffer[256];
                snprintf(buffer, sizeof(buffer),
                         "%s -> atteso %d, ottenuto %d",
                         label,
                         expected,
                         actual);
                fail_test(buffer);
        }
}

static void expect_size(const char* label, size_t actual, size_t expected) {
        if (actual == expected) {
                pass_test(label);
        } else {
                char buffer[256];
                snprintf(buffer, sizeof(buffer),
                         "%s -> atteso %zu, ottenuto %zu",
                         label,
                         expected,
                         actual);
                fail_test(buffer);
        }
}

static void expect_ptr_value(const char* label, void* ptr, int expected) {
        if (ptr == NULL) {
                char buffer[256];
                snprintf(buffer, sizeof(buffer),
                         "%s -> atteso puntatore a %d, ottenuto NULL",
                         label,
                         expected);
                fail_test(buffer);
                return;
        }

        if (*(int*)ptr == expected) {
                pass_test(label);
        } else {
                char buffer[256];
                snprintf(buffer, sizeof(buffer),
                         "%s -> atteso %d, ottenuto %d",
                         label,
                         expected,
                         *(int*)ptr);
                fail_test(buffer);
        }
}

static void run_null_pointer_tests(void) {
        AVLTree tree = NULL;
        bool resultBool = false;
        size_t resultSize = 999;
        int resultHeight = 999;
        void* resultPtr = NULL;
        status_codes res;

        begin_test("Robustezza su puntatori NULL");

        res = avl_create(NULL);
        expect_status("avl_create(NULL)", res, ERROR_NULL_POINTER);

        res = avl_destroy(NULL);
        expect_status("avl_destroy(NULL)", res, ERROR_NULL_POINTER);

        res = avl_search(NULL, NULL, NULL, NULL);
        expect_status("avl_search con tutti NULL", res, ERROR_NULL_POINTER);

        res = avl_insert(NULL, NULL, NULL);
        expect_status("avl_insert con tree NULL", res, ERROR_NULL_POINTER);

        res = avl_remove(NULL, NULL, NULL);
        expect_status("avl_remove con tree NULL", res, ERROR_NULL_POINTER);

        res = avl_is_empty(NULL, &resultBool);
        expect_status("avl_is_empty tree NULL", res, ERROR_NULL_POINTER);

        res = avl_size(NULL, &resultSize);
        expect_status("avl_size tree NULL", res, ERROR_NULL_POINTER);

        res = avl_height(NULL, &resultHeight);
        expect_status("avl_height tree NULL", res, ERROR_NULL_POINTER);

        res = avl_min(NULL, &resultPtr);
        expect_status("avl_min tree NULL", res, ERROR_NULL_POINTER);

        res = avl_max(NULL, &resultPtr);
        expect_status("avl_max tree NULL", res, ERROR_NULL_POINTER);

        res = avl_print(NULL, print_int);
        expect_status("avl_print tree NULL", res, ERROR_NULL_POINTER);

        res = avl_print_inorder(NULL, print_int);
        expect_status("avl_print_inorder tree NULL", res, ERROR_NULL_POINTER);

        res = avl_create(&tree);
        if (res != OK) {
                fail_test("impossibile creare albero nei test NULL");
                return;
        }

        res = avl_insert(tree, NULL, NULL);
        expect_status("avl_insert cmp NULL e value NULL", res, ERROR_NULL_POINTER);

        res = avl_search(tree, NULL, cmp_int, &resultBool);
        expect_status("avl_search value NULL", res, ERROR_NULL_POINTER);

        res = avl_search(tree, &(int){1}, NULL, &resultBool);
        expect_status("avl_search cmp NULL", res, ERROR_NULL_POINTER);

        res = avl_search(tree, &(int){1}, cmp_int, NULL);
        expect_status("avl_search result NULL", res, ERROR_NULL_POINTER);

        res = avl_is_empty(tree, NULL);
        expect_status("avl_is_empty result NULL", res, ERROR_NULL_POINTER);

        res = avl_size(tree, NULL);
        expect_status("avl_size result NULL", res, ERROR_NULL_POINTER);

        res = avl_height(tree, NULL);
        expect_status("avl_height result NULL", res, ERROR_NULL_POINTER);

        res = avl_min(tree, NULL);
        expect_status("avl_min result NULL", res, ERROR_NULL_POINTER);

        res = avl_max(tree, NULL);
        expect_status("avl_max result NULL", res, ERROR_NULL_POINTER);

        res = avl_print(tree, NULL);
        expect_status("avl_print callback NULL", res, ERROR_NULL_POINTER);

        res = avl_print_inorder(tree, NULL);
        expect_status("avl_print_inorder callback NULL", res, ERROR_NULL_POINTER);

        res = avl_destroy(&tree);
        expect_status("avl_destroy su albero vuoto", res, OK);
    }

static void run_empty_tree_tests(void) {
        AVLTree tree = NULL;
        status_codes res;
        bool isEmpty = false;
        size_t size = 111;
        int height = 111;
        void* ptr = NULL;

        begin_test("Comportamento su albero vuoto");

        res = avl_create(&tree);
        if (res != OK) {
                fail_test("impossibile creare albero vuoto");
                return;
        }

        res = avl_is_empty(tree, &isEmpty);
        expect_status("avl_is_empty su albero vuoto", res, OK);
        if (res == OK) expect_bool("is_empty == true", isEmpty, true);

        res = avl_size(tree, &size);
        expect_status("avl_size su albero vuoto", res, OK);
        if (res == OK) expect_size("size == 0", size, 0);

        res = avl_height(tree, &height);
        expect_status("avl_height su albero vuoto", res, OK);
        if (res == OK) expect_int("height == -1", height, -1);

        res = avl_min(tree, &ptr);
        expect_status("avl_min su albero vuoto", res, ERROR_EMPTY_TREE);

        res = avl_max(tree, &ptr);
        expect_status("avl_max su albero vuoto", res, ERROR_EMPTY_TREE);

        res = avl_remove(tree, cmp_int, &(int){10});
        expect_status("avl_remove su albero vuoto", res, ERROR_NOT_FOUND);

        res = avl_print(tree, print_int);
        expect_status("avl_print su albero vuoto", res, OK);

        res = avl_print_inorder(tree, print_int);
        expect_status("avl_print_inorder su albero vuoto", res, OK);

        res = avl_destroy(&tree);
        expect_status("avl_destroy finale albero vuoto", res, OK);
    }

static void run_basic_insert_search_tests(void) {
        AVLTree tree = NULL;
        status_codes res;
        bool found = false;
        size_t size = 0;
        int height = 0;
        void* ptr = NULL;

        int* p10 = alloc_int(10);
        int* p20 = alloc_int(20);
        int* p5 = alloc_int(5);
        int* p10dup = alloc_int(10);

        track_ptr(p10);
        track_ptr(p20);
        track_ptr(p5);
        track_ptr(p10dup);

        begin_test("Insert, search, min, max, size, height");

        res = avl_create(&tree);
        if (res != OK) {
                fail_test("impossibile creare albero base");
                return;
        }

        res = avl_insert(tree, cmp_int, p10);
        expect_status("insert 10", res, OK);

        res = avl_insert(tree, cmp_int, p20);
        expect_status("insert 20", res, OK);

        res = avl_insert(tree, cmp_int, p5);
        expect_status("insert 5", res, OK);

        res = avl_insert(tree, cmp_int, p10dup);
        expect_status("insert duplicato 10", res, NODE_ALREADY_EXISTS);

        res = avl_search(tree, &(int){10}, cmp_int, &found);
        expect_status("search 10", res, OK);
        if (res == OK) expect_bool("10 presente", found, true);

        res = avl_search(tree, &(int){999}, cmp_int, &found);
        expect_status("search 999", res, OK);
        if (res == OK) expect_bool("999 assente", found, false);

        res = avl_size(tree, &size);
        expect_status("size dopo 3 insert validi", res, OK);
        if (res == OK) expect_size("size == 3", size, 3);

        res = avl_min(tree, &ptr);
        expect_status("avl_min base", res, OK);
        if (res == OK) expect_ptr_value("min == 5", ptr, 5);

        res = avl_max(tree, &ptr);
        expect_status("avl_max base", res, OK);
        if (res == OK) expect_ptr_value("max == 20", ptr, 20);

        res = avl_height(tree, &height);
        expect_status("avl_height base", res, OK);
        if (res == OK) {
                if (height >= 1 && height <= 2) {
                        pass_test("height ragionevole dopo pochi insert");
                } else {
                        fail_test("height fuori range atteso dopo pochi insert");
                }
        }

        res = avl_print(tree, print_int);
        expect_status("avl_print base", res, OK);

        res = avl_print_inorder(tree, print_int);
        expect_status("avl_print_inorder base", res, OK);

        res = avl_destroy(&tree);
        expect_status("destroy albero base", res, OK);
    }

static void run_rotation_like_sequences(void) {
        AVLTree tree = NULL;
        status_codes res;
        bool found = false;
        size_t size = 0;
        int height = 0;
        void* ptr = NULL;

        int i;
        int values1[] = {30, 20, 10}; /* caso LL */
        int values2[] = {10, 20, 30}; /* caso RR */
        int values3[] = {30, 10, 20}; /* caso LR */
        int values4[] = {10, 30, 20}; /* caso RL */

        begin_test("Sequenze che dovrebbero attivare rotazioni");

        res = avl_create(&tree);
        if (res != OK) {
                fail_test("impossibile creare albero rotazioni");
                return;
        }

        for (i = 0; i < 3; i++) {
                int* p = alloc_int(values1[i]);
                track_ptr(p);
                res = avl_insert(tree, cmp_int, p);
                if (res != OK) {
                        fail_test("insert sequenza LL fallita");
                        break;
                }
            }

        res = avl_search(tree, &(int){10}, cmp_int, &found);
        expect_status("search 10 dopo LL", res, OK);
        if (res == OK) expect_bool("10 presente dopo LL", found, true);

        res = avl_search(tree, &(int){20}, cmp_int, &found);
        expect_status("search 20 dopo LL", res, OK);
        if (res == OK) expect_bool("20 presente dopo LL", found, true);

        res = avl_search(tree, &(int){30}, cmp_int, &found);
        expect_status("search 30 dopo LL", res, OK);
        if (res == OK) expect_bool("30 presente dopo LL", found, true);

        res = avl_min(tree, &ptr);
        expect_status("min dopo LL", res, OK);
        if (res == OK) expect_ptr_value("min == 10 dopo LL", ptr, 10);

        res = avl_max(tree, &ptr);
        expect_status("max dopo LL", res, OK);
        if (res == OK) expect_ptr_value("max == 30 dopo LL", ptr, 30);

        res = avl_size(tree, &size);
        expect_status("size dopo LL", res, OK);
        if (res == OK) expect_size("size == 3 dopo LL", size, 3);

        res = avl_height(tree, &height);
        expect_status("height dopo LL", res, OK);
        if (res == OK) {
                if (height <= 2) pass_test("height contenuta dopo LL");
                else fail_test("height troppo alta dopo LL");
        }

        res = avl_destroy(&tree);
        expect_status("destroy dopo LL", res, OK);

        /* RR */
        res = avl_create(&tree);
        if (res != OK) {
                fail_test("impossibile creare albero RR");
                return;
        }
        for (i = 0; i < 3; i++) {
                int* p = alloc_int(values2[i]);
                track_ptr(p);
                res = avl_insert(tree, cmp_int, p);
                if (res != OK) {
                        fail_test("insert sequenza RR fallita");
                        break;
                }
        }
        res = avl_height(tree, &height);
        expect_status("height dopo RR", res, OK);
        if (res == OK) {
                if (height <= 2) pass_test("height contenuta dopo RR");
                else fail_test("height troppo alta dopo RR");
        }
        res = avl_destroy(&tree);
        expect_status("destroy dopo RR", res, OK);

        /* LR */
        res = avl_create(&tree);
        if (res != OK) {
                fail_test("impossibile creare albero LR");
                return;
        }
        for (i = 0; i < 3; i++) {
                int* p = alloc_int(values3[i]);
                track_ptr(p);
                res = avl_insert(tree, cmp_int, p);
                if (res != OK) {
                        fail_test("insert sequenza LR fallita");
                        break;
                }
        }
        res = avl_height(tree, &height);
        expect_status("height dopo LR", res, OK);
        if (res == OK) {
                if (height <= 2) pass_test("height contenuta dopo LR");
                else fail_test("height troppo alta dopo LR");
        }
        res = avl_destroy(&tree);
        expect_status("destroy dopo LR", res, OK);

        /* RL */
        res = avl_create(&tree);
        if (res != OK) {
                fail_test("impossibile creare albero RL");
                return;
        }
        for (i = 0; i < 3; i++) {
                int* p = alloc_int(values4[i]);
                track_ptr(p);
                res = avl_insert(tree, cmp_int, p);
                if (res != OK) {
                        fail_test("insert sequenza RL fallita");
                        break;
                }
        }
        res = avl_height(tree, &height);
        expect_status("height dopo RL", res, OK);
        if (res == OK) {
                if (height <= 2) pass_test("height contenuta dopo RL");
                else fail_test("height troppo alta dopo RL");
        }
        res = avl_destroy(&tree);
        expect_status("destroy dopo RL", res, OK);
    }

static void run_remove_tests(void) {
        AVLTree tree = NULL;
        status_codes res;
        bool found = false;
        size_t size = 0;
        void* ptr = NULL;

        int* p20 = alloc_int(20);
        int* p10 = alloc_int(10);
        int* p30 = alloc_int(30);
        int* p5 = alloc_int(5);
        int* p15 = alloc_int(15);
        int* p25 = alloc_int(25);
        int* p35 = alloc_int(35);

        track_ptr(p20);
        track_ptr(p10);
        track_ptr(p30);
        track_ptr(p5);
        track_ptr(p15);
        track_ptr(p25);
        track_ptr(p35);

        begin_test("Remove: foglia, un figlio, due figli, non trovato");

        res = avl_create(&tree);
        if (res != OK) {
                fail_test("impossibile creare albero remove");
                return;
        }

        res = avl_insert(tree, cmp_int, p20); expect_status("insert 20", res, OK);
        res = avl_insert(tree, cmp_int, p10); expect_status("insert 10", res, OK);
        res = avl_insert(tree, cmp_int, p30); expect_status("insert 30", res, OK);
        res = avl_insert(tree, cmp_int, p5);  expect_status("insert 5", res, OK);
        res = avl_insert(tree, cmp_int, p15); expect_status("insert 15", res, OK);
        res = avl_insert(tree, cmp_int, p25); expect_status("insert 25", res, OK);
        res = avl_insert(tree, cmp_int, p35); expect_status("insert 35", res, OK);

        res = avl_remove(tree, cmp_int, &(int){999});
        expect_status("remove valore non presente", res, ERROR_NOT_FOUND);

        res = avl_remove(tree, cmp_int, &(int){5});
        expect_status("remove foglia 5", res, OK);

        res = avl_search(tree, &(int){5}, cmp_int, &found);
        expect_status("search 5 dopo remove", res, OK);
        if (res == OK) expect_bool("5 assente dopo remove", found, false);

        res = avl_remove(tree, cmp_int, &(int){30});
        expect_status("remove nodo con due figli 30", res, OK);

        res = avl_search(tree, &(int){30}, cmp_int, &found);
        expect_status("search 30 dopo remove", res, OK);
        if (res == OK) expect_bool("30 assente dopo remove", found, false);

        res = avl_remove(tree, cmp_int, &(int){10});
        expect_status("remove nodo interno 10", res, OK);

        res = avl_size(tree, &size);
        expect_status("size dopo remove multipli", res, OK);
        if (res == OK) expect_size("size == 4", size, 4);

        res = avl_min(tree, &ptr);
        expect_status("min dopo remove", res, OK);
        if (res == OK) expect_ptr_value("min dopo remove == 15", ptr, 15);

        res = avl_max(tree, &ptr);
        expect_status("max dopo remove", res, OK);
        if (res == OK) expect_ptr_value("max dopo remove == 35", ptr, 35);

        res = avl_print(tree, print_int);
        expect_status("avl_print dopo remove", res, OK);

        res = avl_destroy(&tree);
        expect_status("destroy dopo remove tests", res, OK);
    }

static void run_bulk_monotonic_insert_test(void) {
        AVLTree tree = NULL;
        status_codes res;
        int height = 0;
        size_t size = 0;
        int i;

        begin_test("Insert monotoni 1..15");

        res = avl_create(&tree);
        if (res != OK) {
                fail_test("impossibile creare albero bulk");
                return;
        }

        for (i = 1; i <= 15; i++) {
                int* p = alloc_int(i);
                track_ptr(p);
                res = avl_insert(tree, cmp_int, p);
                if (res != OK) {
                        char buffer[128];
                        snprintf(buffer, sizeof(buffer), "insert %d fallita", i);
                        fail_test(buffer);
                        break;
                }
        }

        res = avl_size(tree, &size);
        expect_status("size dopo 15 insert", res, OK);
        if (res == OK) expect_size("size == 15", size, 15);

        res = avl_height(tree, &height);
        expect_status("height dopo 15 insert", res, OK);
        if (res == OK) {
                if (height <= 6) {
                        pass_test("height plausibile dopo 15 insert monotoni");
                } else {
                        fail_test("height sospettosamente troppo alta dopo 15 insert monotoni");
                }
        }

        res = avl_destroy(&tree);
        expect_status("destroy bulk", res, OK);
    }

static void run_all_tests(void) {
        run_null_pointer_tests();
        run_empty_tree_tests();
        run_basic_insert_search_tests();
        run_rotation_like_sequences();
        run_remove_tests();
        run_bulk_monotonic_insert_test();

        printf("\n=====================================\n");
        printf("TEST PASSATI: %d\n", testsPassed);
        printf("TEST FALLITI: %d\n", testsFailed);
        printf("=====================================\n");
    }

static void print_manual_menu(void) {
        printf("\n===== MODALITA' INTERATTIVA =====\n");
        printf("1) Inserisci intero\n");
        printf("2) Cerca intero\n");
        printf("3) Elimina intero\n");
        printf("4) Stampa albero\n");
        printf("5) Stampa inorder\n");
        printf("6) Min\n");
        printf("7) Max\n");
        printf("8) Size\n");
        printf("9) Height\n");
        printf("10) Is empty\n");
        printf("11) Esci\n");
        printf("Scelta: ");
    }

static void run_manual_mode(void) {
        AVLTree tree = NULL;
        status_codes res;
        int choice = 0;

        res = avl_create(&tree);
        if (res != OK) {
                printf("Errore avl_create: %s\n", status_to_string(res));
                return;
        }

        do {
                print_manual_menu();

                if (scanf("%d", &choice) != 1) {
                        printf("Input non valido.\n");
                        clear_input_buffer();
                        continue;
                }

                if (choice == 1) {
                        int value;
                        int* p;

                        printf("Valore da inserire: ");
                        if (scanf("%d", &value) != 1) {
                                printf("Input non valido.\n");
                                clear_input_buffer();
                                continue;
                        }

                        p = alloc_int(value);
                        if (p == NULL) {
                                printf("Errore malloc.\n");
                                continue;
                        }
                        track_ptr(p);

                        res = avl_insert(tree, cmp_int, p);
                        printf("avl_insert -> %s\n", status_to_string(res));

                } else if (choice == 2) {
                        int value;
                        bool found = false;

                        printf("Valore da cercare: ");
                        if (scanf("%d", &value) != 1) {
                                printf("Input non valido.\n");
                                clear_input_buffer();
                                continue;
                        }

                        res = avl_search(tree, &value, cmp_int, &found);
                        printf("avl_search -> %s, found=%d\n", status_to_string(res), (int)found);

                } else if (choice == 3) {
                        int value;

                        printf("Valore da eliminare: ");
                        if (scanf("%d", &value) != 1) {
                                printf("Input non valido.\n");
                                clear_input_buffer();
                                continue;
                        }

                        res = avl_remove(tree, cmp_int, &value);
                        printf("avl_remove -> %s\n", status_to_string(res));

                } else if (choice == 4) {
                        res = avl_print(tree, print_int);
                        printf("avl_print -> %s\n", status_to_string(res));

                } else if (choice == 5) {
                        res = avl_print_inorder(tree, print_int);
                        printf("avl_print_inorder -> %s\n", status_to_string(res));

                } else if (choice == 6) {
                        void* ptr = NULL;
                        res = avl_min(tree, &ptr);
                        printf("avl_min -> %s", status_to_string(res));
                        if (res == OK) {
                                printf(", value=");
                                print_int(ptr);
                        }
                        printf("\n");

                } else if (choice == 7) {
                        void* ptr = NULL;
                        res = avl_max(tree, &ptr);
                        printf("avl_max -> %s", status_to_string(res));
                        if (res == OK) {
                                printf(", value=");
                                print_int(ptr);
                        }
                        printf("\n");

                } else if (choice == 8) {
                        size_t size = 0;
                        res = avl_size(tree, &size);
                        printf("avl_size -> %s, size=%zu\n", status_to_string(res), size);

                } else if (choice == 9) {
                        int height = 0;
                        res = avl_height(tree, &height);
                        printf("avl_height -> %s, height=%d\n", status_to_string(res), height);

                } else if (choice == 10) {
                        bool isEmpty = false;
                        res = avl_is_empty(tree, &isEmpty);
                        printf("avl_is_empty -> %s, isEmpty=%d\n", status_to_string(res), (int)isEmpty);

                } else if (choice == 11) {
                        printf("Uscita modalita' interattiva.\n");
                } else {
                        printf("Scelta non valida.\n");
                }

        } while (choice != 11);

        res = avl_destroy(&tree);
        printf("avl_destroy -> %s\n", status_to_string(res));
    }


int main4(void) {


        printf("===== SUITE TEST AVL =====\n");
        run_all_tests();

        printf("\nVuoi entrare anche nella modalita' interattiva? (1 = si, altro = no): ");
        int choice = 0;
        if (scanf("%d", &choice) == 1 && choice == 1) {
                run_manual_mode();
        }

        free_all_tracked_values();
        return 0;
}