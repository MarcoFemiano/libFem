#include <stdio.h>
#include <stdlib.h>

#include "BST_AVL.h"

static int cmp_int(const void* a, const void* b) {
        int valA = *(const int*)a;
        int valB = *(const int*)b;

        if (valA > valB) return 1;
        if (valA < valB) return -1;
        return 0;
}

static const char* status_to_string(status_codes code) {
        switch (code) {
                case OK: return "OK";
                case ERROR_NULL_POINTER: return "ERROR_NULL_POINTER";
                case ERROR_ARITHMETIC_OVERFLOW: return "ERROR_ARITHMETIC_OVERFLOW";
                case ERROR_REALLOC_FAILURE: return "ERROR_REALLOC_FAILURE";
                case ERROR_ALLOCATION_FAILURE: return "ERROR_ALLOCATION_FAILURE";
                case ERROR_EMPTY: return "ERROR_EMPTY";
                case ERROR_NOT_FOUND: return "ERROR_NOT_FOUND";
                case ERROR_INVALID_ARGUMENT: return "ERROR_INVALID_ARGUMENT";
                case NODE_ALREADY_EXISTS: return "NODE_ALREADY_EXISTS";
                default: return "UNKNOWN_STATUS_CODE";
        }
}

static void print_menu(void) {
        printf("\n");
        printf("===== TEST AVL =====\n");
        printf("1) Inserisci intero\n");
        printf("2) Cerca intero\n");
        printf("3) Esci\n");
        printf("Scelta: ");
}

int main(void) {
        AVLTree tree = NULL;
        status_codes result;

        result = avl_create(sizeof(int), &tree);
        if (result != OK) {
                printf("Errore avl_create: %s\n", status_to_string(result));
                return 1;
        }

        int choice = 0;

        do {
                print_menu();

                if (scanf("%d", &choice) != 1) {
                        printf("Input non valido.\n");
                        while (getchar() != '\n');
                        continue;
                }

                if (choice == 1) {
                        int* value = malloc(sizeof(int));
                        if (value == NULL) {
                                printf("Errore malloc.\n");
                                continue;
                        }

                        printf("Valore da inserire: ");
                        if (scanf("%d", value) != 1) {
                                printf("Input non valido.\n");
                                free(value);
                                while (getchar() != '\n');
                                continue;
                        }

                        result = avl_insert(tree, cmp_int, value);

                        if (result == NODE_ALREADY_EXISTS) {
                                printf("Nodo gia' presente.\n");
                                free(value); // importante: l'albero non prende possesso dei duplicati rifiutati
                        } else if (result != OK) {
                                printf("Errore avl_insert: %s\n", status_to_string(result));
                                free(value);
                        } else {
                                printf("Inserimento completato.\n");
                        }

                } else if (choice == 2) {
                        int value;
                        bool found = false;

                        printf("Valore da cercare: ");
                        if (scanf("%d", &value) != 1) {
                                printf("Input non valido.\n");
                                while (getchar() != '\n');
                                continue;
                        }

                        result = avl_search(tree, &value, cmp_int, &found);
                        if (result != OK) {
                                printf("Errore avl_search: %s\n", status_to_string(result));
                        } else {
                                if (found) printf("Valore trovato.\n");
                                else printf("Valore NON trovato.\n");
                        }

                } else if (choice == 3) {
                        printf("Uscita...\n");
                } else {
                        printf("Scelta non valida.\n");
                }

        } while (choice != 3);

        result = avl_destroy(&tree);
        if (result != OK) {
                printf("Errore avl_destroy: %s\n", status_to_string(result));
                return 1;
        }

        return 0;
}