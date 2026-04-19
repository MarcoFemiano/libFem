#include <stdio.h>
#include <stdlib.h>

#include "BST_AVL.h"

#define MAX_TRACKED_VALUES 1024

static int* trackedValues[MAX_TRACKED_VALUES];
static size_t trackedCount = 0;

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
        printf("3) Elimina intero\n");
        printf("4) Stampa albero\n");
        printf("5) Stampa inorder\n");
        printf("6) Esci\n");
        printf("Scelta: ");
}

static void clear_input_buffer(void) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
}

static bool track_value_pointer(int* ptr) {
        if (trackedCount >= MAX_TRACKED_VALUES) return false;

        trackedValues[trackedCount] = ptr;
        trackedCount++;
        return true;
}

static int* find_tracked_pointer_by_value(int value) {
        size_t i;

        for (i = 0; i < trackedCount; i++) {
                if (trackedValues[i] != NULL && *trackedValues[i] == value) {
                        return trackedValues[i];
                }
        }

        return NULL;
}

static void untrack_pointer(int* ptr) {
        size_t i;

        for (i = 0; i < trackedCount; i++) {
                if (trackedValues[i] == ptr) {
                        trackedValues[i] = NULL;
                        return;
                }
        }
}

static void free_all_tracked_values(void) {
        size_t i;

        for (i = 0; i < trackedCount; i++) {
                if (trackedValues[i] != NULL) {
                        free(trackedValues[i]);
                        trackedValues[i] = NULL;
                }
        }

        trackedCount = 0;
}

static void print_int(const void* value) {
        if (value == NULL) {
                printf("NULL");
                return;
        }

        printf("%d", *(const int*)value);
}

int main(void) {
        AVLTree tree = NULL;
        status_codes result;
        int choice = 0;

        result = avl_create(sizeof(int), &tree);
        if (result != OK) {
                printf("Errore avl_create: %s\n", status_to_string(result));
                return 1;
        }

        do {
                print_menu();

                if (scanf("%d", &choice) != 1) {
                        printf("Input non valido.\n");
                        clear_input_buffer();
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
                                clear_input_buffer();
                                continue;
                        }

                        result = avl_insert(tree, cmp_int, value);

                        if (result == NODE_ALREADY_EXISTS) {
                                printf("Nodo gia' presente.\n");
                                free(value);
                        } else if (result != OK) {
                                printf("Errore avl_insert: %s\n", status_to_string(result));
                                free(value);
                        } else {
                                if (track_value_pointer(value) == false) {
                                        printf("Inserito, ma il tracking del test e' pieno.\n");
                                }
                                printf("Inserimento completato.\n");
                        }

                } else if (choice == 2) {
                        int value;
                        bool found = false;

                        printf("Valore da cercare: ");
                        if (scanf("%d", &value) != 1) {
                                printf("Input non valido.\n");
                                clear_input_buffer();
                                continue;
                        }

                        result = avl_search(tree, &value, cmp_int, &found);
                        if (result != OK) {
                                printf("Errore avl_search: %s\n", status_to_string(result));
                        } else {
                                if (found) {
                                        printf("Valore trovato.\n");
                                } else {
                                        printf("Valore NON trovato.\n");
                                }
                        }

                } else if (choice == 3) {
                        int value;
                        int* ptrToRemove;

                        printf("Valore da eliminare: ");
                        if (scanf("%d", &value) != 1) {
                                printf("Input non valido.\n");
                                clear_input_buffer();
                                continue;
                        }

                        ptrToRemove = find_tracked_pointer_by_value(value);
                        if (ptrToRemove == NULL) {
                                printf("Valore non presente tra quelli tracciati dal test.\n");
                                continue;
                        }

                        result = avl_remove(tree, cmp_int, ptrToRemove);
                        if (result != OK) {
                                printf("Errore avl_remove: %s\n", status_to_string(result));
                        } else {
                                printf("Eliminazione completata.\n");
                                untrack_pointer(ptrToRemove);
                                free(ptrToRemove);
                        }

                } else if (choice == 4) {
                        result = avl_print(tree, print_int);
                        if (result != OK) {
                                printf("Errore avl_print: %s\n", status_to_string(result));
                        }

                } else if (choice == 5) {
                        result = avl_print_inorder(tree, print_int);
                        if (result != OK) {
                                printf("Errore avl_print_inorder: %s\n", status_to_string(result));
                        }

                } else if (choice == 6) {
                        printf("Uscita...\n");
                }
        } while (choice != 6);

        result = avl_destroy(&tree);
        if (result != OK) {
                printf("Errore avl_destroy: %s\n", status_to_string(result));
                free_all_tracked_values();
                return 1;
        }

        free_all_tracked_values();

        return 0;
}