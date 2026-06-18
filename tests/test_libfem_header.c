#include "libfem.h"
#include "test_support.h"

static int int_cmp(const void* a, const void* b) {
  if (a == NULL || b == NULL) return -1;

  int x = *(const int*)a;
  int y = *(const int*)b;

  if (x < y) return -1;
  if (x > y) return 1;
  return 0;
}

int main(void) {
  Stack stack = NULL;
  Coda coda = NULL;
  HashTable table = NULL;
  AVLTree tree = NULL;
  Graph graph = NULL;
  LinkedList list = NULL;
  int value = 1;

  test_expect_true("stack create", (stack = stack_create(1, sizeof(int))) != NULL);
  test_expect_true("coda create", (coda = coda_create(1, sizeof(int))) != NULL);
  test_expect_int("hash table create", hashTable_create(&table, 4, sizeof(int)), HT_OK);
  test_expect_int("hash table push", hashTable_push(table, int_cmp, &value), HT_OK);
  test_expect_int("avl create", avl_create(&tree), OK);
  test_expect_int("graph create", graph_create(&graph, 1, true), OK);
  test_expect_true("linked list create", (list = linkedList_create()) != NULL);
  test_expect_int("linked list add", linkedList_add_tail(list, &value), LINKED_LIST_OK);

  test_expect_int("linked list destroy", linkedList_destroy(&list), LINKED_LIST_OK);
  test_expect_int("graph destroy", graph_destroy(&graph), OK);
  test_expect_int("avl destroy", avl_destroy(&tree), OK);
  test_expect_int("hash table destroy", hashTable_destroy(&table), HT_OK);
  test_expect_int("coda destroy", coda_destroy(&coda), OK);
  stack_destroy(&stack);
  test_expect_true("stack destroy", stack == NULL);

  return test_result();
}
