//
// Created by Spara on 18/04/2026.
//

#include "BST_AVL.h"

#include <assert.h>
#include <stdlib.h>


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
        size_t sizeOfVals;
};

//region helpers
static int max_int(int a, int b) {
        return (a > b) ? a : b;
}

//endregion

status_codes avl_create(size_t sizeOfVals, AVLTree* tree) {
        //tests di robustezza
        if (sizeOfVals == 0) return ERROR_INVALID_ARGUMENT;
        if (tree == NULL) return ERROR_NULL_POINTER;

        AVLTree temp = malloc(sizeof(struct strAVLTree));
        if (temp == NULL) return ERROR_ALLOCATION_FAILURE;
        *tree = temp;
        (*tree)->sizeOfVals = sizeOfVals;
        (*tree)->root = NULL;
        return OK;
}
/*
status_codes temp(AVLTree tree) {
        if (tree == NULL) return ERROR_ALLOCATION_FAILURE;

        tree = malloc(sizeof(struct strNodo));


        (*tree).sizeOfVals = sizeOfVals;
        (*tree).val = NULL;
        (*tree).left = NULL;
        (*tree).right = NULL;
        (*tree).altezzaSx = 0;
        (*tree).altezzaDx = 0;
        (*tree).balance = 0;
}*/

static void node_destroy(Nodo node) {
        //test di robustezza
        if (node == NULL) return;

        node_destroy(node->leftChild);
        node_destroy(node->rightChild);

        free(node);
}

status_codes avl_destroy(AVLTree* tree) {
        //tests di robustezza
        if (tree == NULL || *tree == NULL) return ERROR_NULL_POINTER;

        node_destroy((*tree)->root);

        free(*tree);
        *tree = NULL;

        return OK;
}





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
        }else if (cmpResult > 0) {//vado a destra
                return avl_search_node(node->rightChild,value,cmp,result);
        }else{ //vado a sinistra
                return avl_search_node(node->leftChild,value,cmp,result);
        }
}

status_codes avl_search(AVLTree tree,const void* value,int (*cmp)(const void*,const void*), bool* result) {
        //tests di robustezza
        if (tree == NULL) return ERROR_NULL_POINTER;
        if (cmp == NULL) return ERROR_NULL_POINTER;
        if (result == NULL) return ERROR_NULL_POINTER;
        if (value == NULL) return ERROR_NULL_POINTER;

        return avl_search_node(tree->root,value,cmp,result);
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

status_codes avl_insert(AVLTree tree,int (*cmp)(const void*,const void*), void* value) {
        if (tree == NULL) return ERROR_NULL_POINTER;
        if (value == NULL) return ERROR_NULL_POINTER;
        if (cmp == NULL) return ERROR_NULL_POINTER;

        return avl_insert_node(&(tree->root),cmp,value);

}

