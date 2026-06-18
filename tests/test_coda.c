#include "coda.h"
#include "test_support.h"

int main(void) {
  Coda coda = NULL;
  int out = 0;

  coda = coda_create(2, sizeof(int));
  test_expect_true("create coda", coda != NULL);
  test_expect_int("pop empty", coda_pop(coda, &out), ERROR_CODA_EMPTY);

  int one = 1;
  int two = 2;
  int three = 3;
  test_expect_int("push 1", coda_push(coda, &one), OK);
  test_expect_int("push 2", coda_push(coda, &two), OK);
  test_expect_int("push full", coda_push(coda, &three), ERROR_CODA_FULL);
  test_expect_int("peek", coda_peek(coda, &out), OK);
  test_expect_int("peek == 1", out, 1);
  test_expect_int("pop", coda_pop(coda, &out), OK);
  test_expect_int("pop == 1", out, 1);
  test_expect_int("push after wrap", coda_push(coda, &three), OK);
  test_expect_int("size == 2", (int)coda_size(coda), 2);
  test_expect_int("clear", coda_clear(coda), OK);
  test_expect_int("empty", coda_is_empty(coda), 1);
  test_expect_int("destroy", coda_destroy(&coda), OK);

  return test_result();
}
