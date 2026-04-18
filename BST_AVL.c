//
// Created by Spara on 18/04/2026.
//

#include "BST_AVL.h"

#include <assert.h>
#include <stdlib.h>


struct strNodo {

        void* val;

        struct strNodo *left;
        struct strNodo *right;

        int altezzaSx;
        int altezzaDx;
        int balance;
};

struct strAVLTree {
        struct strNodo *root;
        size_t sizeOfVals;
};





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

        if (node->val != NULL) free(node->val);

        node_destroy(node->left);
        node_destroy(node->right);

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
        if (node == NULL) {
                *result = false;
                return OK;
        }


        if (node->val == NULL) {
                *result = false;
                return OK;
        }

        int cmpResult = cmp(value,node->val);
        if (cmpResult == 0) {
                *result = true; //trovato
                return OK;
        }else if (cmpResult > 0) {
                return avl_search_node(node->right,value,cmp,result);
        }else{
                return avl_search_node(node->left,value,cmp,result);
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