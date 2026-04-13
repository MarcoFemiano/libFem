//
// Created by Spara on 13/04/2026.
//

#ifndef LIBFEM_STACK_H
#define LIBFEM_STACK_H

typedef struct strStack* Stack;


Stack stack_create(Stack stack);
void stack_destroy(Stack* stack);
short stack_push(Stack stack, void* dato);
short stack_pop(Stack stack, void* dato);
short stack_peek(Stack stack, void* dato);
unsigned long long int stack_size(Stack stack);
short stack_is_empty(Stack stack);
short stack_is_full(Stack stack);




#endif //LIBFEM_STACK_H