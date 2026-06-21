//
// Created by Spara on 18/04/2026.
//

#include "BST_AVL.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LOOKING_FOR_MIN 0
#define LOOKING_FOR_MAX 1
struct strNodo {

        void* val;

        struct strNodo *leftChild;
        struct strNodo *rightChild;

        int leftHeight;
        int rightHeight;
        int balance;
};

struct strAVLTree {
        struct strNodo *root;
        size_t size;
        int height;
};

//region structs for threads

typedef struct strThreadAttraversalArgs {
        Coda coda;
        Nodo node;
}ThreadAttraversalArgs;
//endregion


//region private prototypes
static status_codes avl_is_node_balanced(Nodo node, bool* result );
static int max_int(int a, int b);
static Nodo* avl_find_leftmost_node(Nodo* node);
static void node_destroy(Nodo node);
static status_codes avl_search_node(Nodo node, const void* value, int (*cmp) (const void*,const void*), bool* result);
static void avl_update_balance(Nodo* node);
static Nodo avl_balance_node(Nodo* node);
static status_codes avl_insert_node(Nodo* node,int (*cmp) (const void*, const void*), void* value);
static status_codes avl_remove_node(Nodo* node, int (*cmp)(const void*, const void*), void* value);
static void avl_print_node(Nodo node, void (*print_value)(const void*), int depth);
static void avl_print_inorder_node(Nodo node, void (*print_value)(const void*));
static status_codes max_height(const int* sx,const int* dx, int* result);
static status_codes min_max_node(Nodo node, void** result,bool scelta);
static void* avl_DFS_Thread_attraversalMultiThread(void* arg);
static void avl_DFS_Father_attraversalMultiThread(Nodo node,Coda coda, unsigned int numT, int* cont, pthread_t** thread);

//endregion




status_codes avl_create(AVLTree* tree) {
        //tests di robustezza

        if (tree == NULL) return ERROR_NULL_POINTER;

        AVLTree temp = malloc(sizeof(struct strAVLTree));
        if (temp == NULL) return ERROR_ALLOCATION_FAILURE;
        *tree = temp;
        (*tree)->root = NULL;
        (*tree)->size = 0;
        (*tree)->height = -1;
        return OK;
}




status_codes avl_destroy(AVLTree* tree) {
        //tests di robustezza
        if (tree == NULL || *tree == NULL) return ERROR_NULL_POINTER;

        node_destroy((*tree)->root);

        free(*tree);
        *tree = NULL;

        return OK;
}







status_codes avl_search(AVLTree tree,const void* value,int (*cmp)(const void*,const void*), bool* result) {
        //tests di robustezza
        if (tree == NULL) return ERROR_NULL_POINTER;
        if (cmp == NULL) return ERROR_NULL_POINTER;
        if (result == NULL) return ERROR_NULL_POINTER;
        if (value == NULL) return ERROR_NULL_POINTER;

        return avl_search_node(tree->root,value,cmp,result);
}







status_codes avl_insert(AVLTree tree,int (*cmp)(const void*,const void*), void* value) {
        if (tree == NULL) return ERROR_NULL_POINTER;
        if (value == NULL) return ERROR_NULL_POINTER;
        if (cmp == NULL) return ERROR_NULL_POINTER;

        status_codes res =  avl_insert_node(&(tree->root),cmp,value);

        if (res != OK) return res;

        tree->size++;
        return OK;

}






status_codes avl_remove(AVLTree tree, int (*cmp)(const void*, const void*), void* value) {
        //tests di robustezza
        if (tree == NULL) return ERROR_NULL_POINTER;
        if (cmp == NULL) return ERROR_NULL_POINTER;
        if (value == NULL) return ERROR_NULL_POINTER;

        status_codes res = avl_remove_node(&tree->root, cmp, value);

        if (res != OK) return res;

        tree->size--;
        return OK;
}






status_codes avl_print(AVLTree tree, void (*print_value)(const void*)) {
        //tests di robustezza
        if (tree == NULL) return ERROR_NULL_POINTER;
        if (print_value == NULL) return ERROR_NULL_POINTER;

        printf("\n===== AVL TREE =====\n");

        if (tree->root == NULL) {
                printf("(empty)\n");
                return OK;
        }

        avl_print_node(tree->root, print_value, 0);

        return OK;
}



status_codes avl_print_inorder(AVLTree tree, void (*print_value)(const void*)) {
        //tests di robustezza
        if (tree == NULL) return ERROR_NULL_POINTER;
        if (print_value == NULL) return ERROR_NULL_POINTER;

        printf("\n===== AVL INORDER =====\n");

        if (tree->root == NULL) {
                printf("(empty)\n");
                return OK;
        }

        avl_print_inorder_node(tree->root, print_value);
        printf("\n");

        return OK;
}





status_codes avl_is_empty(AVLTree tree, bool* result) {
        //tests di robustezza
        if (tree == NULL) return ERROR_NULL_POINTER;
        if (result == NULL) return ERROR_NULL_POINTER;

        *result = (bool) (tree->root == NULL);
        return OK;
}


status_codes avl_size(AVLTree tree,size_t* result) {
        if (tree == NULL) return ERROR_NULL_POINTER;
        if (result == NULL) return ERROR_NULL_POINTER;

        *result = tree->size;
        return OK;
}



status_codes avl_height(AVLTree tree,int* result) {
        //tests di robustezza
        if (result == NULL) return ERROR_NULL_POINTER;
        if (tree == NULL) return ERROR_NULL_POINTER;

        if (tree->root == NULL) {
                *result = -1;
                return OK;
        }
        status_codes res = max_height(&tree->root->leftHeight, &tree->root->rightHeight, result);
        if (res != OK) return res;
        *result +=1 ;

        return OK;
}


status_codes avl_min(AVLTree tree,void** result) {
        //tests di robustezza
        if (tree == NULL) return ERROR_NULL_POINTER;
        if (result == NULL) return ERROR_NULL_POINTER;
        if (tree->root == NULL) return ERROR_EMPTY_TREE;

       status_codes res = min_max_node(tree->root, result,LOOKING_FOR_MIN);
        if (res != OK) return res;

        return OK;
}

status_codes avl_max(AVLTree tree,void** result) {
        //tests di robustezza
        if (tree == NULL) return ERROR_NULL_POINTER;
        if (result == NULL) return ERROR_NULL_POINTER;
        if (tree->root == NULL) return ERROR_EMPTY_TREE;

        status_codes res = min_max_node(tree->root, result,LOOKING_FOR_MAX);
        if (res != OK) return res;

        return OK;
}




status_codes avl_is_balanced(AVLTree tree, bool* result) {
        //tests di robustezza
        if (tree == NULL) return ERROR_NULL_POINTER;
        if (result == NULL) return ERROR_NULL_POINTER;
        *result = true;
        if (tree->root == NULL) {

                return OK;
        }

        status_codes res = avl_is_node_balanced(tree->root,result);
        if (res != OK) return res;




        return OK;
}




status_codes avl_DFS_attraversalMultiThread(AVLTree tree, unsigned int numThreads) {
        //tests di robustezza
        if (tree == NULL || tree->root == NULL) return ERROR_NULL_POINTER;
        if (numThreads <2) return ERROR_INVALID_ARGUMENT;

        Coda coda = coda_create(0,sizeof(struct strNodo));
        if (coda == NULL) return ERROR_ALLOCATION_FAILURE;

        ThreadAttraversalArgs arg;
        arg.coda = coda;
        arg.node = tree->root;

        int* cont = malloc(sizeof(int));
        if (cont == NULL) {
                free(coda);
                return ERROR_ALLOCATION_FAILURE;
        }
        *cont = 0;

        pthread_t* threads = calloc(numThreads,sizeof(pthread_t));
        if (threads == NULL) {
                free(cont);
                free(coda);
                return ERROR_ALLOCATION_FAILURE;
        }

        avl_DFS_Father_attraversalMultiThread(tree->root,coda,numThreads,cont,&threads);

        int *num = malloc(sizeof(int));
        if (num == NULL) {
                free(cont);
                free(coda);
                free(threads);
                return ERROR_ALLOCATION_FAILURE;
        }
        *num = 0;
        printf("stampo l'attraversamento\n");
        while (coda_pop(coda,num) == OK ) {
                printf("[%d] ",*num);
        }
        printf("\n");

        free(cont);
        free(coda);
        free(threads);
        free(num);

        for (unsigned int i = 0; i<numThreads;i++) {
                pthread_join(threads[i],NULL);
        }

        return OK;
}






//region helpers
static int max_int(int a, int b) {
        return (a > b) ? a : b;
}

static Nodo* avl_find_leftmost_node(Nodo* node) {
        //tests di robustezza
        if (node == NULL) return NULL;
        if (*node == NULL) return NULL;

        while ((*node)->leftChild != NULL) {
                node = &(*node)->leftChild;
        }

        return node;
}
//endregion


static status_codes avl_is_node_balanced(Nodo node, bool* result ) {
        //test di robustezza
        if (result == NULL) return ERROR_NULL_POINTER;

        if (node == NULL) return OK;
        //base case
        if (abs(node->balance)> 1 ) {
                *result = false;
                return OK;
        }

        status_codes res =  avl_is_node_balanced(node->leftChild,result);
        if (res != OK || !*result) return res;

        status_codes res2 = avl_is_node_balanced(node->rightChild,result);
        if (res2 != OK || !*result) return res2;

        return OK;
}




static status_codes min_max_node(Nodo node, void** result,bool scelta) {
        if (node == NULL) return ERROR_NULL_POINTER;
        if (result == NULL) return ERROR_NULL_POINTER;

        if (scelta == LOOKING_FOR_MIN) {
                //base case
                if (node->leftChild == NULL) {
                        *result = node->val;
                        return OK;
                }else {
                        return min_max_node(node->leftChild, result, scelta);
                }

        }

        if (scelta == LOOKING_FOR_MAX)
        {
                //base case
                if (node->rightChild == NULL) {
                        *result = node->val;
                        return OK;
                }else {
                        return min_max_node(node->rightChild, result, scelta);
                }
        }

        //scelta è diverso da true e false
        return ERROR_INVALID_ARGUMENT;

}

static void node_destroy(Nodo node) {
        //test di robustezza
        if (node == NULL) return;

        node_destroy(node->leftChild);
        node_destroy(node->rightChild);

        free(node);
}


static status_codes max_height(const int* sx,const int* dx, int* result) {
        if (sx == NULL) return ERROR_NULL_POINTER;
        if (dx == NULL) return ERROR_NULL_POINTER;
        if (result == NULL) return ERROR_NULL_POINTER;

        if (*sx >= *dx) {
                *result = *sx;
                return OK;
        }else {
                *result = *dx;
                return OK;
        }

        return OK;
}


static void avl_print_inorder_node(Nodo node, void (*print_value)(const void*)) {
        //base case
        if (node == NULL) return;

        avl_print_inorder_node(node->leftChild, print_value);
        print_value(node->val);
        printf(" ");
        avl_print_inorder_node(node->rightChild, print_value);
}


static void avl_print_node(Nodo node, void (*print_value)(const void*), int depth) {
        //base case
        if (node == NULL) return;

        avl_print_node(node->rightChild, print_value, depth + 1);

        for (int i = 0; i < depth; i++) {
                printf("        ");
        }

        print_value(node->val);
        printf(" [B=%d|L=%d|R=%d]\n", node->balance, node->leftHeight, node->rightHeight);

        avl_print_node(node->leftChild, print_value, depth + 1);
}


static status_codes avl_remove_node(Nodo* node, int (*cmp)(const void*, const void*), void* value) {
        //tests di robustezza
        if (node == NULL) return ERROR_NULL_POINTER;
        if (cmp == NULL) return ERROR_NULL_POINTER;
        if (value == NULL) return ERROR_NULL_POINTER;

        //base case: nodo non trovato
        if (*node == NULL) return ERROR_NOT_FOUND;

        status_codes result;
        int cmpResult = cmp(value, (*node)->val);

        if (cmpResult > 0) {//vado a dx
                result = avl_remove_node(&(*node)->rightChild, cmp, value);
                if (result != OK) return result;

                avl_balance_node(node);
                return OK;

        }else if (cmpResult < 0) {//vado a sx
                result = avl_remove_node(&(*node)->leftChild, cmp, value);
                if (result != OK) return result;

                avl_balance_node(node);
                return OK;
        }

        //caso in cui il nodo è stato trovato

        //caso non ha figli
        if ((*node)->leftChild == NULL && (*node)->rightChild == NULL) {
                free(*node);
                *node = NULL;
                return OK;
        }

        //caso ha solo il figlio sinistro
        if ((*node)->leftChild != NULL && (*node)->rightChild == NULL) {
                Nodo nodeToDelete = *node;
                *node = (*node)->leftChild;
                free(nodeToDelete);
                return OK;
        }

        //caso ha solo il figlio destro
        if ((*node)->leftChild == NULL && (*node)->rightChild != NULL) {
                Nodo nodeToDelete = *node;
                *node = (*node)->rightChild;
                free(nodeToDelete);
                return OK;
        }

        //caso in cui ha entrambi i figli
        //prendo il successore inorder:
        //il nodo più a sinistra del sottoalbero destro
        Nodo* leftmostNode = avl_find_leftmost_node(&(*node)->rightChild);
        if (leftmostNode == NULL || *leftmostNode == NULL) return ERROR_NOT_FOUND;

        (*node)->val = (*leftmostNode)->val;

        Nodo nodeToDelete = *leftmostNode;
        *leftmostNode = (*leftmostNode)->rightChild;
        free(nodeToDelete);

        avl_balance_node(node);

        return OK;
}


static status_codes avl_insert_node(Nodo* node,int (*cmp) (const void*, const void*), void* value) {
        //tests di robustezza
        if (cmp == NULL) return ERROR_NULL_POINTER;
        if (value == NULL) return ERROR_NULL_POINTER;
        if (node == NULL) return ERROR_NULL_POINTER;

        //base case
        if (*node == NULL) {
                Nodo newNode = malloc(sizeof(struct strNodo));
                if (newNode == NULL) return ERROR_ALLOCATION_FAILURE;

                newNode->val = value;
                newNode->leftChild = NULL;
                newNode->rightChild = NULL;
                newNode->balance = 0;
                newNode->leftHeight = -1;
                newNode->rightHeight = -1;
                *node = newNode;


        }else if (cmp(value,(*node)->val) > 0) {//vado a dx
                status_codes res = avl_insert_node(&(*node)->rightChild,cmp,value);
                if (res != OK) return res;
        }else if (cmp(value,(*node)->val) < 0) {//vado a sx
                status_codes res = avl_insert_node(&(*node)->leftChild,cmp,value);
                if (res != OK) return res;
        }else return NODE_ALREADY_EXISTS;

        avl_balance_node(node);

        return OK;
}


//region Balance -------------------------------------------------------------




static void avl_update_balance(Nodo* node) {
        //tests di robustezza
        if (node == NULL || *node == NULL) return;

        if ((*node)->rightChild == NULL) {
                (*node)->rightHeight = -1;
        }else {
                (*node)->rightHeight = 1 + max_int((*node)->rightChild->leftHeight,(*node)->rightChild->rightHeight);
        }

        if ((*node)->leftChild == NULL) {
                (*node)->leftHeight = -1;
        }else {
                (*node)->leftHeight =  1 + max_int((*node)->leftChild->leftHeight,(*node)->leftChild->rightHeight);
        }

        (*node)->balance = (*node)->leftHeight - (*node)->rightHeight;
}

static Nodo avl_balance_node(Nodo* node) {
        //tests di robustezza
        if (node == NULL || *node == NULL) return (node == NULL) ? NULL : *node;

        avl_update_balance(node);

        if (abs((*node)->balance) > 1) {
                //nodo sbilanciato
                // aggiorno il balance dei figli (se esistono) e
                // faccio le rotazioni

                if ((*node)->rightChild != NULL) {
                        avl_update_balance(&(*node)->rightChild);
                }

                if ((*node)->leftChild != NULL) {
                        avl_update_balance(&(*node)->leftChild);
                }

                if ((*node)->balance > 0) { //caso in cui è sbilanciato a sinistra

                        if ((*node)->leftChild->balance >= 0) {
                                //caso in cui il figlio è
                                //sbilanciato nella stessa direzione del padre
                                //rotazione singola
                                //sto lavorando a sinistra perchè è sbilanciato a sinistra

                                Nodo ptrA = (*node);
                                Nodo ptrB = (*node)->leftChild;
                                Nodo ptrC = ptrB->leftChild;
                                Nodo ptrD = ptrB->rightChild;

                                (*node) = ptrB; //B prende il posto di A
                                (*node)->rightChild = ptrA; //A diventa figlio destro di B
                                (*node)->leftChild = ptrC; //C diventa figlio sinistro di B
                                (*node)->rightChild->leftChild = ptrD; //il figlio destro di B diventa figlio sinistro di A

                                avl_update_balance(&(*node)->leftChild);
                                avl_update_balance(&(*node)->rightChild);
                                avl_update_balance(node);

                        }else {
                                //caso in cui il figlio è sbilanciato
                                //nella direzione diversa dal padre
                                //rotazione doppia
                                //sto lavorando a sinistra perchè è sbilanciato a sinistra

                                Nodo ptrA = (*node);
                                Nodo ptrB = (*node)->leftChild;
                                Nodo ptrC = ptrB->rightChild;
                                Nodo ptrX = ptrC->leftChild;
                                Nodo ptrY = ptrC->rightChild;

                                (*node) = ptrC; //C prende il posto di A
                                ptrA->leftChild = ptrY; //Y diventa figlio sinistro di A
                                ptrB->rightChild = ptrX; //X diventa il figlio destro di B
                                (*node)->rightChild = ptrA; //A diventa figlio destro di C
                                (*node)->leftChild = ptrB; //B Diventa figlio sinistro di C

                                avl_update_balance(&(*node)->leftChild);
                                avl_update_balance(&(*node)->rightChild);
                                avl_update_balance(node);
                        }

                }else { //caso in cui è sbilanciato a destra

                        if ((*node)->rightChild->balance <= 0) {
                                //caso in cui il figlio è
                                //sbilanciato nella stessa direzione del padre
                                //rotazione singola
                                //sto lavorando a destra

                                Nodo ptrA = (*node);
                                Nodo ptrB = (*node)->rightChild;
                                Nodo ptrC = ptrB->rightChild;
                                Nodo ptrD = ptrB->leftChild;

                                (*node) = ptrB; //B prende il posto di A
                                (*node)->rightChild = ptrC; //C diventa figlio destro di B
                                (*node)->leftChild = ptrA; //A diventa figlio sinistro di B
                                ptrA->rightChild = ptrD; //D diventa figlio destro di A

                                avl_update_balance(&(*node)->leftChild);
                                avl_update_balance(&(*node)->rightChild);
                                avl_update_balance(node);

                        }else {
                                //caso in cui il figlio è sbilanciato
                                //nella direzione diversa dal padre
                                //rotazione doppia
                                //lavoro a destra

                                Nodo ptrA = (*node);
                                Nodo ptrB = (*node)->rightChild;
                                Nodo ptrC = ptrB->leftChild;
                                Nodo ptrX = ptrC->leftChild;
                                Nodo ptrY = ptrC->rightChild;

                                (*node) = ptrC; //C diventa padre
                                (*node)->rightChild = ptrB; //B diventa figlio destro di C
                                (*node)->leftChild = ptrA; //A diventa figlio sinistro di C
                                ptrA->rightChild = ptrX; //X diventa figlio destro di A
                                ptrB->leftChild = ptrY; //Y diventa figlio sinistro di B

                                avl_update_balance(&(*node)->leftChild);
                                avl_update_balance(&(*node)->rightChild);
                                avl_update_balance(node);
                        }
                }
        }

        return *node;
}
//endregion

static status_codes avl_search_node(Nodo node, const void* value, int (*cmp) (const void*,const void*), bool* result) {
        //base case
        if (node == NULL) {
                *result = false;
                return OK;
        }

        int cmpResult = cmp(value,node->val);
        if (cmpResult == 0) {
                *result = true; //trovato
                return OK;
        }else if (cmpResult == 1) {//vado a destra
                return avl_search_node(node->rightChild,value,cmp,result);
        }else if (cmpResult == -1){ //vado a sinistra
                return avl_search_node(node->leftChild,value,cmp,result);
        }else return ERROR_NULL_POINTER;
}




static void avl_DFS_Father_attraversalMultiThread(Nodo node,Coda coda, unsigned int numT, int* cont, pthread_t** thread) {
        //caso base
        if (node == NULL) return;
        //MANCA LA GESTIONE DELL'ERRORE SU QUESTA MALLOC

        //ricorsione
        if (numT < *cont) {

                ThreadAttraversalArgs* arg = malloc(sizeof( struct strThreadAttraversalArgs));
                arg->node = node->leftChild;
                arg->coda = coda;
                pthread_create(thread[*cont],NULL,avl_DFS_Thread_attraversalMultiThread,arg);
                (*cont)++;
        }else {
                avl_DFS_Father_attraversalMultiThread(node->leftChild,coda,numT,cont,thread);
        }

        avl_DFS_Father_attraversalMultiThread(node->rightChild,coda,numT,cont,thread);


}

static void* avl_DFS_Thread_attraversalMultiThread(void* arg) {

        ThreadAttraversalArgs *targ = (ThreadAttraversalArgs *)arg;

        //caso Base
        if (targ->node== NULL) return NULL;

        //visita
        coda_push(targ->coda,targ->node);

        //ricorsione
        if (targ->node->leftChild != NULL) {
                targ->node = targ->node->leftChild;
                avl_DFS_Thread_attraversalMultiThread(targ);
        }

        if (targ->node->rightChild != NULL){
                targ->node = targ->node->rightChild;
                avl_DFS_Thread_attraversalMultiThread(targ);
        }

        return NULL;
}
