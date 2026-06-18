#include "hash_table.h"
#include "test_support.h"

static int int_cmp(const void* a, const void* b) {
  if (a == NULL || b == NULL) return -2;
  int x = *(const int*)a;
  int y = *(const int*)b;
  if (x < y) return -1;
  if (x > y) return 1;
  return 0;
}

int main(void) {
  HashTable table = NULL;
  bool found = false;
  int value = 42;
  int other = 7;

  test_expect_int("create ok", hashTable_create(&table, 4, sizeof(int)), HT_OK);
  test_expect_int("push value", hashTable_push(table, int_cmp, &value), HT_OK);
  test_expect_int("duplicate", hashTable_push(table, int_cmp, &value), HT_ERROR_VALUE_ALREADY_EXISTS);
  test_expect_int("search value", hashTable_search(table, int_cmp, &value, &found), HT_OK);
  test_expect_int("found true", found, true);
  test_expect_int("search absent", hashTable_search(table, int_cmp, &other, &found), HT_OK);
  test_expect_int("found false", found, false);
  test_expect_int("remove value", hashTable_remove(table, int_cmp, &value), HT_OK);
  test_expect_int("remove absent", hashTable_remove(table, int_cmp, &value), HT_ELEMENTO_NON_TROVATO);
  test_expect_int("destroy", hashTable_destroy(&table), HT_OK);

  return test_result();
}
