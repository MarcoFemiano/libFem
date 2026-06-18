#include "stack.h"
#include "test_support.h"

int main(void) {
  Stack stack = NULL;
  int out = 0;

  stack = stack_create(2, sizeof(int));
  test_expect_true("create stack", stack != NULL);
  test_expect_int("pop empty", stack_pop(stack, &out), ERROR_STACK_EMPTY);

  int one = 1;
  int two = 2;
  int three = 3;
  test_expect_int("push 1", stack_push(stack, &one), OK);
  test_expect_int("push 2", stack_push(stack, &two), OK);
  test_expect_int("push full", stack_push(stack, &three), ERROR_STACK_FULL);
  test_expect_size("size == 2", stack_size(stack), 2);
  test_expect_int("peek", stack_peek(stack, &out), OK);
  test_expect_int("peek == 2", out, 2);
  test_expect_int("pop", stack_pop(stack, &out), OK);
  test_expect_int("pop == 2", out, 2);
  test_expect_int("clear", stack_clear(stack), OK);
  test_expect_int("empty", stack_is_empty(stack), 1);

  stack_destroy(&stack);
  test_expect_true("destroy sets NULL", stack == NULL);

  return test_result();
}
