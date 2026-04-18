//
// Created by Spara on 13/04/2026.
//

/**
 * @file stack.c
 * @brief Implementazione dell'ADT Stack generico.
 *
 * Questo file contiene l'implementazione privata di uno stack generico
 * basato su buffer contiguo e copia raw degli elementi.
 */

#include "stack.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>



/**
 * @brief Struttura concreta privata dello stack.
 *
 * La struttura mantiene:
 * - il buffer degli elementi
 * - la dimensione logica corrente
 * - la capacità del buffer
 * - l'informazione sul comportamento statico/dinamico
 * - la dimensione di ciascun elemento in byte
 */
struct strStack {
    /**
     * @brief Buffer contiguo contenente gli elementi dello stack.
     *
     * Il buffer è interpretato come sequenza di byte.
     */
    void* dati;

    /**
     * @brief Numero di elementi attualmente presenti nello stack.
     */
    size_t size;

    /**
     * @brief Capacità corrente dello stack espressa in numero di elementi.
     */
    size_t capacity;

    /**
     * @brief Modalità dello stack.
     *
     * - 0: stack a capacità fissa
     * - 1: stack dinamico
     */
    bool dinamico;

    /**
     * @brief Dimensione, in byte, di ogni elemento memorizzato.
     */
    size_t sizeOfEachElement;
};




Stack stack_create( size_t capacity, size_t sizeOfEachElement) {

    //test di robustezza
    if (sizeOfEachElement == 0) return NULL;

    Stack stack = malloc(sizeof (struct strStack));
    //test di robustezza
    if (stack == NULL) return NULL;



    stack->sizeOfEachElement = sizeOfEachElement;
    stack->capacity = capacity;
    stack->size = 0;
    //Se capacity è > 0 voglio uno stack a dimensione fissa, quindi alloco capacity elementi
    if (stack->capacity > 0) {

        //test di robustezza per overflow aritmetico
        if (capacity > SIZE_MAX / sizeOfEachElement) {
            free(stack);
            return NULL;
        }
        stack->dati = malloc(capacity * sizeOfEachElement);

        if (stack->dati == NULL) {
            free(stack);
            return NULL;
        }

        stack->dinamico = false;
    }
    //Se capacity è 0 voglio uno stack a dimensione dinamica potenzialmente illimitato
    else if (stack->capacity == 0) {
        //test di robustezza
        if (sizeOfEachElement > SIZE_MAX / DEFAULT_SIZE) {
            free(stack);
            return NULL;
        }
        //inizializzo con DEFAULT_SIZE elementi
        stack->dati = malloc(DEFAULT_SIZE * sizeOfEachElement);
        stack->capacity = DEFAULT_SIZE;

        if (stack->dati == NULL) {
            free(stack);
            return NULL;
        }

        stack -> dinamico = true;
    }


    return stack;

}


void stack_destroy(Stack* stack) {
    //test di robustezza
    if (stack == NULL || *stack == NULL) return;

    free((*stack)->dati);
    free(*stack);
    *stack = NULL;

}


short stack_push(Stack stack,const void* datoInput) {
    //tests di robustezza
    if (stack == NULL) return ERROR_NULL_POINTER;
    if (datoInput == NULL) return ERROR_NULL_DATA_POINTER;

    if (stack->dinamico) {

        //se ho raggiunto la massimo capienza, allargo geometricamente
        if (stack->size >= stack->capacity) {
            //test di robustezza per overflow aritmetico
            if (stack->capacity > SIZE_MAX / 2 / stack->sizeOfEachElement) return ERROR_ARITHMETIC_OVERFLOW;

            void *temp = realloc(stack->dati, (stack->capacity*2) * stack->sizeOfEachElement);

            if (temp == NULL) return ERROR_REALLOC_FAIL;

            stack->dati = temp;
            stack->capacity *=2;

        }

    }else if (stack->size >= stack->capacity) return ERROR_STACK_FULL;

    memcpy((unsigned char*)stack->dati + (stack->sizeOfEachElement * stack->size), datoInput, stack->sizeOfEachElement);
    stack->size++;

    return OK;
}



short stack_pop(Stack stack, void* datoOutput)
{
    //tests di robustezza
    if (stack == NULL) return ERROR_NULL_POINTER;
    if (datoOutput == NULL) return ERROR_NULL_DATA_POINTER;

    if (stack->size == 0) return ERROR_STACK_EMPTY;

    stack->size--;
    memcpy( datoOutput,(unsigned char*) stack->dati + (stack->sizeOfEachElement * stack->size), stack->sizeOfEachElement);


    return OK;
}


short stack_peek(Stack stack, void* datoOutput) {

    //tests di robustezza
    if (stack == NULL) return ERROR_NULL_POINTER;
    if (datoOutput == NULL) return ERROR_NULL_DATA_POINTER;

    if (stack->size == 0) return ERROR_STACK_EMPTY;


    memcpy( datoOutput,(unsigned char*) stack->dati + (stack->sizeOfEachElement * (stack->size-1)), stack->sizeOfEachElement);


    return OK;

}


size_t stack_size(Stack stack) {
    //test di robustezza
    if (stack == NULL) return 0;

    return stack->size;
}

short stack_is_empty(Stack stack) {
    if (stack == NULL) return ERROR_NULL_POINTER;

    return (short) (stack->size == 0);

}

short stack_is_full(Stack stack) {
    if (stack == NULL) return ERROR_NULL_POINTER;

    if (stack->dinamico) return ERROR_FUNCTION_NOT_VALID_IN_THIS_USE_CASE;

    return (short) (stack->size == stack->capacity);
}

short stack_clear_and_resize_to_default(Stack stack) {
    if (stack == NULL) return ERROR_NULL_POINTER;
    if (!stack->dinamico) return ERROR_FUNCTION_NOT_VALID_IN_THIS_USE_CASE;

    if (stack->sizeOfEachElement > SIZE_MAX / DEFAULT_SIZE) return ERROR_ARITHMETIC_OVERFLOW;
    void* temp = realloc(stack->dati, DEFAULT_SIZE * stack->sizeOfEachElement);

    if (temp == NULL) return ERROR_REALLOC_FAIL;
    stack->dati = temp;
    stack->capacity = DEFAULT_SIZE;

    stack->size = 0;

    return OK;
}


short stack_clear(Stack stack) {
    if (stack == NULL) return ERROR_NULL_POINTER;

    stack->size = 0;
    return OK;
}