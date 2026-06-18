//
// Created by Spara on 16/06/2026.
//

#include "LinkedList.h"

#include <stdlib.h>
typedef struct strNode* listNode;

struct strNode {
    void* val;
    struct strNode *next;
};

/*
 * Invarianti interne della lista:
 * - lista vuota: head == NULL, tail == NULL, size == 0;
 * - lista non vuota: head != NULL, tail != NULL, tail->next == NULL;
 * - se size == 1 allora head == tail;
 * - seguendo next da head si devono visitare esattamente size nodi;
 * - ogni nodo contiene un val non NULL;
 * - i dati puntati da val non sono posseduti dalla lista.
 */
struct strLinkedList {
    listNode head;
    listNode tail;
    size_t size;
};

typedef struct strSearchResult {
    listNode prev;
    listNode current;
}search_result;


//region private prototypes
static search_result* linkedList_internal_search(LinkedList linkedList,int (*cmp)(const void*,const void*), void* val);
static LinkedList_statusCodes linkedList_internal_check_arithmetic_overflow(LinkedList linkedList);

//endregion




const char* linkedList_print_statusCodes(LinkedList_statusCodes status_codes) {

    switch (status_codes) {

        case LINKED_LIST_OK:
            return "OK";
            break;

        case LINKED_LIST_VALUE_FOUND:
            return "Found";
            break;

        case LINKED_LIST_VALUE_NOT_FOUND:
            return "Not Found";
            break;

        case LINKED_LIST_ITS_ALREADY_NULL:
            return "Already Null.";
            break;

        case LINKED_LIST_ERROR_NULL_POINTER:
            return "Error: Null Pointer given.";
            break;

        case LINKED_LIST_ERROR_ALLOCATION_FAILURE:
            return "Error: Allocation Failed.";
            break;

        case LINKED_LIST_ARITHMETIC_OVERFLOW:
            return "Error: Arithmetic Overflow.";
            break;

        case LINKED_LIST_NEGATIVE_INDEX:
            return "Error: Negative index given.";
            break;

        case LINKED_LIST_SEMANTIC_CONTRADDICTION:
            return "Logic Error.";
            break;

        case LINKED_LIST_INDEX_POINTING_TO_AN_UNCHAINED_NODE:
            return "Error: Index given points to a non existent list position.";
            break;

        case LINKED_LIST_INVALID_INDEX:
            return "Error: invalid index.";
            break;

        case LINKED_LIST_EMPTY_LIST:
            return "Error: List is empty.";
            break;

        default:
            return "Unknown error occurred.";
            break;
    }
}


LinkedList linkedList_create(void) {

    LinkedList list = malloc(sizeof( struct strLinkedList));

    if (list == NULL) return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;

}

LinkedList_statusCodes linkedList_destroy(LinkedList* linkedList) {

    if (linkedList == NULL || *linkedList == NULL) return LINKED_LIST_ERROR_NULL_POINTER;

    listNode current = (*linkedList)->head;

    while (current != NULL) {
        current = current->next;
        free((*linkedList)->head);
        (*linkedList)->head = current;
    }

    free(*linkedList);
    *linkedList = NULL;
    return LINKED_LIST_OK;
}

LinkedList_statusCodes linkedList_clear(LinkedList linkedList) {

    if (linkedList == NULL) return LINKED_LIST_ERROR_NULL_POINTER;

    listNode current = linkedList->head;
    while (current != NULL) {
        current = current->next;
        free(linkedList->head);
        linkedList->head = current;
    }

    linkedList->size = 0;
    linkedList->tail = NULL;

    return LINKED_LIST_OK;
}


LinkedList_statusCodes linkedList_add_head(LinkedList linkedList,void* val) {

    if (linkedList == NULL || val == NULL) return LINKED_LIST_ERROR_NULL_POINTER;
    LinkedList_statusCodes arithmeticOverflowCheck = linkedList_internal_check_arithmetic_overflow(linkedList);
    if (arithmeticOverflowCheck != LINKED_LIST_OK) return arithmeticOverflowCheck;

    listNode node = malloc(sizeof(struct strNode));
    if (node == NULL) return LINKED_LIST_ERROR_ALLOCATION_FAILURE;

    node->val = val;

    if (linkedList->head == NULL) {
        linkedList->head = node;
        linkedList->tail = node;
        node->next = NULL;
    }else {
        node->next = linkedList->head;
        linkedList->head = node;
    }

    linkedList->size +=1;
    return LINKED_LIST_OK;
}


LinkedList_statusCodes linkedList_add_tail(LinkedList linkedList,void* val) {

    if (linkedList == NULL || val == NULL) return LINKED_LIST_ERROR_NULL_POINTER;
    LinkedList_statusCodes arithmeticOverflowCheck = linkedList_internal_check_arithmetic_overflow(linkedList);
    if (arithmeticOverflowCheck != LINKED_LIST_OK) return arithmeticOverflowCheck;
    listNode node = malloc(sizeof(struct strNode));
    if (node == NULL) return LINKED_LIST_ERROR_ALLOCATION_FAILURE;

    node->val = val;
    node->next = NULL;

    if (linkedList->tail == NULL) {
        linkedList->head = node;
        linkedList->tail = node;
    }else {
        linkedList->tail->next = node;
        linkedList->tail = node;
    }

    linkedList->size +=1;
    return LINKED_LIST_OK;
}


LinkedList_statusCodes linkedList_add_at(LinkedList linkedList,void* val, int index) {

    if (linkedList == NULL || val == NULL) return LINKED_LIST_ERROR_NULL_POINTER;
    if (index < 0) return LINKED_LIST_NEGATIVE_INDEX;
    if ((size_t)index > linkedList->size) return LINKED_LIST_INVALID_INDEX;
    if (index == 0) return linkedList_add_head(linkedList,val);
    if ((size_t)index == linkedList->size) return linkedList_add_tail(linkedList,val);
    if (linkedList->head == NULL) return LINKED_LIST_SEMANTIC_CONTRADDICTION;
    LinkedList_statusCodes arithmeticOverflowCheck = linkedList_internal_check_arithmetic_overflow(linkedList);
    if (arithmeticOverflowCheck != LINKED_LIST_OK ) return arithmeticOverflowCheck;

    listNode node = malloc(sizeof(struct strNode));
    if (node == NULL) return LINKED_LIST_ERROR_ALLOCATION_FAILURE;

    node->val = val;
    listNode current = linkedList->head;
    for (int i = 1; i<index; i++) current = current->next;
    node->next = current->next;
    current->next = node;
    linkedList->size +=1;

    return LINKED_LIST_OK;

}


LinkedList_statusCodes linkedList_remove_head(LinkedList linkedList) {

    if (linkedList == NULL || linkedList->head == NULL) return LINKED_LIST_ERROR_NULL_POINTER;

    if (linkedList->size == 1) linkedList->tail = NULL;

    listNode node = linkedList->head->next;
    free(linkedList->head);
    linkedList->head = node;

    linkedList->size -=1;
    if (linkedList->size == 1) linkedList->tail = linkedList->head;


    return LINKED_LIST_OK;
}

LinkedList_statusCodes linkedList_remove_tail(LinkedList linkedList) {

    if (linkedList == NULL || linkedList->head == NULL) return LINKED_LIST_ERROR_NULL_POINTER;
    if (linkedList->tail == NULL) return LINKED_LIST_ITS_ALREADY_NULL;
    if (linkedList->size == 0) return LINKED_LIST_SEMANTIC_CONTRADDICTION;

    if (linkedList->size == 1) {
        free(linkedList->tail);
        linkedList->head = NULL;
        linkedList->tail = NULL;
        linkedList->size = 0;
        return LINKED_LIST_OK;
    }

    listNode current = linkedList->head;
    while (current->next != linkedList->tail) current = current->next;
    listNode oldTail = linkedList->tail;
    linkedList->tail = current;
    linkedList->tail->next = NULL;
    free(oldTail);
    linkedList->size -=1;

    if (linkedList->size == 1) linkedList->head = linkedList->tail;

    return LINKED_LIST_OK;
}


LinkedList_statusCodes linkedList_remove_at(LinkedList linkedList, int index) {
    if (linkedList == NULL) return LINKED_LIST_ERROR_NULL_POINTER;
    if (index < 0) return LINKED_LIST_NEGATIVE_INDEX;
    if ((size_t)index >= linkedList->size) return LINKED_LIST_INVALID_INDEX;
    if (linkedList->head == NULL) return LINKED_LIST_SEMANTIC_CONTRADDICTION;

    if ((size_t)index == linkedList->size -1) return linkedList_remove_tail(linkedList);
    if (index == 0) return linkedList_remove_head(linkedList);

    listNode current = linkedList->head;
    for (int i = 1; i<index; i++) current = current->next;
    listNode nodePrev = current;
    current = current->next;
    nodePrev->next = current->next;
    linkedList->size -=1;
    free(current);

    return LINKED_LIST_OK;

}

void* linkedList_search(LinkedList linkedList,int (*cmp)(const void*,const void*), void* val) {

    if (linkedList == NULL || val == NULL || cmp == NULL || linkedList->head == NULL) return NULL;

    listNode current = linkedList->head;

    while (current != NULL) {
        if (cmp(current->val,val) == 0) return current->val;
        current = current->next;
    }

    return NULL;
}




LinkedList_statusCodes linkedList_remove_val(LinkedList linkedList,int (*cmp)(const void*,const void*), void* val) {
    if (linkedList == NULL || val == NULL || cmp == NULL) return LINKED_LIST_ERROR_NULL_POINTER;
    if (linkedList->size < 1) return LINKED_LIST_EMPTY_LIST;

    search_result* result = linkedList_internal_search(linkedList,cmp,val);
    if (result == NULL) return LINKED_LIST_VALUE_NOT_FOUND;

    if (result->prev == NULL) {
        free(result);
        return linkedList_remove_head(linkedList);
    }

    if (result->current == linkedList->tail) linkedList->tail = result->prev;
    result->prev->next = result->current->next;
    free(result->current);
    free(result);
    linkedList->size -=1;
    return LINKED_LIST_OK;

}



void* linkedList_search_index(LinkedList linkedList, int index) {
    if (linkedList == NULL) return NULL;
    if (linkedList->head == NULL) return NULL;
    if (index < 0 || (size_t)index >= linkedList->size) return NULL;

    listNode current = linkedList->head;

    for (int i = 0; i<index; i++) current = current->next;


    return current->val;
}


bool linkedList_isEmpty(LinkedList linkedList) {
    if (linkedList == NULL) return true;

    if (linkedList->size == 0) return true;
    else return false;
}

size_t linkedList_size(LinkedList linkedList) {
    if (linkedList == NULL) return 0;

    return linkedList->size;
}






















//region private functions implementation

static search_result* linkedList_internal_search(LinkedList linkedList,int (*cmp)(const void*,const void*), void* val) {
    if (linkedList == NULL || val == NULL || cmp == NULL || linkedList->head == NULL) return NULL;

    search_result* result= malloc(sizeof(search_result));
    if (result == NULL) return NULL;

    listNode current = linkedList->head;
    listNode prev = NULL;
    while (current != NULL) {

        if (cmp(current->val,val) == 0) {
            result->prev = prev;
            result->current = current;
            return result;
        }

        prev = current;
        current = current->next;
    }

    free(result);
    return NULL;
}


static LinkedList_statusCodes linkedList_internal_check_arithmetic_overflow(LinkedList linkedList) {
    if (linkedList == NULL) return LINKED_LIST_ERROR_NULL_POINTER;
    if (linkedList->size == (size_t)-1) return LINKED_LIST_ARITHMETIC_OVERFLOW;

    return LINKED_LIST_OK;
}
//endregion
