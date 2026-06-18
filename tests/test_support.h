#ifndef LIBFEM_TEST_SUPPORT_H
#define LIBFEM_TEST_SUPPORT_H

#include <stddef.h>
#include <stdio.h>

static int testFailures = 0;

static void test_expect_int(const char* label, int actual, int expected) {
  if (actual == expected) {
    printf("[OK] %s\n", label);
  }else {
    printf("[FAIL] %s: atteso %d, ottenuto %d\n", label, expected, actual);
    testFailures++;
  }
}

static void test_expect_size(const char* label, size_t actual, size_t expected) {
  if (actual == expected) {
    printf("[OK] %s\n", label);
  }else {
    printf("[FAIL] %s: atteso %zu, ottenuto %zu\n", label, expected, actual);
    testFailures++;
  }
}

static void test_expect_true(const char* label, int condition) {
  if (condition) {
    printf("[OK] %s\n", label);
  }else {
    printf("[FAIL] %s\n", label);
    testFailures++;
  }
}

static int test_result(void) {
  if (testFailures == 0) {
    printf("TEST OK\n");
    return 0;
  }

  printf("TEST FALLITI: %d\n", testFailures);
  return 1;
}

#endif /* LIBFEM_TEST_SUPPORT_H */
