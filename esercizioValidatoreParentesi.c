//
// main.c - Test per la libreria coda
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coda.h"
#include "stack.h"





















static char inverti_parentesi(char c) {
    switch (c) {
        case '(': return ')';
        case ')': return '(';
        case '[': return ']';
        case ']': return '[';
        case '{': return '}';
        case '}': return '{';
        default:  return c; // se non è parentesi, ritorna uguale
    }
}


int main(void) {



   Stack stack;
    stack = stack_create(0,sizeof(char));

    char formula[] = "][";

    int i = 0;


    while (formula[i] != '\0') {

        char valoreLettoDalloStack;
        short ris = stack_peek(stack,&valoreLettoDalloStack);
        if (ris == OK) {

            if (valoreLettoDalloStack == inverti_parentesi(formula[i])) {
                stack_pop(stack,&valoreLettoDalloStack);
                i++;
                continue;
            }

            if (formula[i] == '(' || formula[i] == '[' || formula[i] == '{') stack_push(stack, &formula[i]);
            else return -1;



        }else if (ris == ERROR_STACK_EMPTY) {
            if (formula[i] == ')' || formula[i] == ']' || formula[i] == '}') return -1;
            stack_push(stack, &formula[i]);
        }

        i++;
    }
    short ris = stack_is_empty(stack);
    if (ris != 1) return -1;
    printf("formula corretta\n");
    return 0;
}