#include "BST_AVL.h"
#include "test_support.h"

#include <stdio.h>
#include <stdlib.h>

typedef enum {
  INSERT_ORDER_ASC = 0,
  INSERT_ORDER_DESC = 1,
  INSERT_ORDER_PSEUDO_RANDOM = 2
} InsertOrder;

static int int_cmp(const void* a, const void* b) {
  int x = *(const int*)a;
  int y = *(const int*)b;
  if (x < y) return -1;
  if (x > y) return 1;
  return 0;
}

static size_t gcd_size(size_t a, size_t b) {
  while (b != 0) {
    size_t temp = b;
    b = a % b;
    a = temp;
  }

  return a;
}

static size_t pseudo_random_step(size_t n) {
  size_t step = (n / 2) + 1;

  if (step == 0) step = 1;

  while (gcd_size(step, n) != 1) {
    step++;
    if (step > n) step = 1;
  }

  return step;
}

static size_t value_for_insert(size_t index, size_t n, InsertOrder order) {
  if (order == INSERT_ORDER_DESC) {
    return n - index;
  }

  if (order == INSERT_ORDER_PSEUDO_RANDOM) {
    size_t step = pseudo_random_step(n);
    return ((index * step) % n) + 1;
  }

  return index + 1;
}

static const char* order_name(InsertOrder order) {
  if (order == INSERT_ORDER_DESC) return "desc";
  if (order == INSERT_ORDER_PSEUDO_RANDOM) return "pseudo-random";
  return "asc";
}

static status_codes fill_tree(AVLTree tree, int* values, size_t n, InsertOrder order) {
  for (size_t i = 1; i <= n; i++) {
    values[i] = (int)i;
  }

  for (size_t i = 0; i < n; i++) {
    size_t value = value_for_insert(i, n, order);
    status_codes res = avl_insert(tree, int_cmp, &values[value]);
    if (res != OK) return res;
  }

  return OK;
}

static int verify_output(Coda coda, size_t n, size_t* countResult, size_t* missingResult, size_t* duplicateResult) {
  int* seen = calloc(n + 1, sizeof(int));
  int output = 0;
  size_t count = 0;
  size_t missing = 0;
  size_t duplicate = 0;

  if (seen == NULL) return ERROR_ALLOCATION_FAILURE;

  while (coda_pop(coda, &output) == OK) {
    count++;
    if (output < 1 || (size_t)output > n) {
      duplicate++;
    }else if (seen[output]) {
      duplicate++;
    }else {
      seen[output] = 1;
    }
  }

  for (size_t i = 1; i <= n; i++) {
    if (!seen[i]) missing++;
  }

  free(seen);

  *countResult = count;
  *missingResult = missing;
  *duplicateResult = duplicate;

  return OK;
}

static int run_concurrent_case(size_t n, unsigned int numThreads, InsertOrder order, const char* prefix) {
  AVLTree tree = NULL;
  Coda coda = NULL;
  int* values = NULL;
  size_t count = 0;
  size_t missing = 0;
  size_t duplicate = 0;
  status_codes res;
  char label[160];
  int ok = true;

  values = calloc(n + 1, sizeof(int));
  if (values == NULL) {
    snprintf(label, sizeof(label), "%s alloc values n=%zu threads=%u order=%s", prefix, n, numThreads, order_name(order));
    test_expect_int(label, ERROR_ALLOCATION_FAILURE, OK);
    return false;
  }

  res = avl_create(&tree);
  if (res != OK) {
    snprintf(label, sizeof(label), "%s create n=%zu threads=%u order=%s", prefix, n, numThreads, order_name(order));
    test_expect_int(label, res, OK);
    free(values);
    return false;
  }

  res = fill_tree(tree, values, n, order);
  if (res != OK) {
    snprintf(label, sizeof(label), "%s fill n=%zu threads=%u order=%s", prefix, n, numThreads, order_name(order));
    test_expect_int(label, res, OK);
    avl_destroy(&tree);
    free(values);
    return false;
  }

  coda = coda_create(0, sizeof(int));
  if (coda == NULL) {
    snprintf(label, sizeof(label), "%s coda n=%zu threads=%u order=%s", prefix, n, numThreads, order_name(order));
    test_expect_int(label, ERROR_ALLOCATION_FAILURE, OK);
    avl_destroy(&tree);
    free(values);
    return false;
  }

  res = avl_DFS_attraversalMultiThread(tree, numThreads, coda);
  if (res != OK) ok = false;

  if (verify_output(coda, n, &count, &missing, &duplicate) != OK) ok = false;
  if (count != n || missing != 0 || duplicate != 0) ok = false;

  if (!ok) {
    printf("[INFO] %s n=%zu threads=%u order=%s res=%d count=%zu missing=%zu duplicate=%zu\n",
           prefix, n, numThreads, order_name(order), res, count, missing, duplicate);
  }

  snprintf(label, sizeof(label), "%s n=%zu threads=%u order=%s", prefix, n, numThreads, order_name(order));
  test_expect_int(label, ok, true);

  coda_destroy(&coda);
  avl_destroy(&tree);
  free(values);

  return ok;
}

static void test_api_contract(void) {
  AVLTree tree = NULL;
  Coda coda = NULL;
  int value = 42;

  coda = coda_create(0, sizeof(int));
  test_expect_int("dfs null tree", avl_DFS_attraversalMultiThread(NULL, 2, coda), ERROR_NULL_POINTER);

  test_expect_int("create api tree", avl_create(&tree), OK);
  test_expect_int("dfs empty tree", avl_DFS_attraversalMultiThread(tree, 2, coda), ERROR_NULL_POINTER);
  test_expect_int("insert api value", avl_insert(tree, int_cmp, &value), OK);
  test_expect_int("dfs null output", avl_DFS_attraversalMultiThread(tree, 2, NULL), ERROR_NULL_POINTER);
  test_expect_int("dfs zero threads", avl_DFS_attraversalMultiThread(tree, 0, coda), ERROR_INVALID_ARGUMENT);
  test_expect_int("dfs one thread", avl_DFS_attraversalMultiThread(tree, 1, coda), ERROR_INVALID_ARGUMENT);

  coda_destroy(&coda);
  avl_destroy(&tree);
}

static void test_correctness_matrix(void) {
  size_t sizes[] = {1, 2, 3, 10, 31, 100, 1000, 5000};
  unsigned int threads[] = {2, 3, 6, 16, 32};
  InsertOrder orders[] = {INSERT_ORDER_ASC, INSERT_ORDER_DESC, INSERT_ORDER_PSEUDO_RANDOM};

  for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
    for (size_t j = 0; j < sizeof(threads) / sizeof(threads[0]); j++) {
      for (size_t k = 0; k < sizeof(orders) / sizeof(orders[0]); k++) {
        run_concurrent_case(sizes[i], threads[j], orders[k], "dfs matrix");
      }
    }
  }
}

static void test_stress_repeated(void) {
  for (int i = 0; i < 100; i++) {
    run_concurrent_case(1000, 8, INSERT_ORDER_PSEUDO_RANDOM, "dfs stress");
  }
}

static void test_static_coda_diagnostic(void) {
  AVLTree tree = NULL;
  Coda coda = NULL;
  int values[11];
  int output = 0;
  int count = 0;
  status_codes res;

  test_expect_int("diagnostic create tree", avl_create(&tree), OK);
  test_expect_int("diagnostic fill tree", fill_tree(tree, values, 10, INSERT_ORDER_ASC), OK);

  coda = coda_create(1, sizeof(int));
  if (coda == NULL) {
    test_expect_int("diagnostic coda create", ERROR_ALLOCATION_FAILURE, OK);
    avl_destroy(&tree);
    return;
  }

  res = avl_DFS_attraversalMultiThread(tree, 4, coda);
  while (coda_pop(coda, &output) == OK) {
    count++;
  }

  printf("[INFO] coda statica piccola: res=%d count=%d su 10; oggi la DFS non propaga ERROR_CODA_FULL\n", res, count);

  coda_destroy(&coda);
  avl_destroy(&tree);
}

int main(void) {
  test_api_contract();
  test_correctness_matrix();
  test_stress_repeated();
  test_static_coda_diagnostic();

  return test_result();
}
