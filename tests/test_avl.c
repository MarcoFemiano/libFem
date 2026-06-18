#include "BST_AVL.h"
#include "test_support.h"

static int int_cmp(const void* a, const void* b) {
  int x = *(const int*)a;
  int y = *(const int*)b;
  if (x < y) return -1;
  if (x > y) return 1;
  return 0;
}

int main(void) {
  AVLTree tree = NULL;
  bool found = false;
  bool balanced = false;
  size_t size = 0;
  void* ptr = NULL;
  int values[] = {30, 10, 20, 40};

  test_expect_int("create ok", avl_create(&tree), OK);
  test_expect_int("insert 30", avl_insert(tree, int_cmp, &values[0]), OK);
  test_expect_int("insert 10", avl_insert(tree, int_cmp, &values[1]), OK);
  test_expect_int("insert 20", avl_insert(tree, int_cmp, &values[2]), OK);
  test_expect_int("insert 40", avl_insert(tree, int_cmp, &values[3]), OK);
  test_expect_int("duplicate", avl_insert(tree, int_cmp, &values[0]), NODE_ALREADY_EXISTS);
  test_expect_int("search 20", avl_search(tree, &values[2], int_cmp, &found), OK);
  test_expect_int("found true", found, true);
  test_expect_int("balanced", avl_is_balanced(tree, &balanced), OK);
  test_expect_int("balanced true", balanced, true);
  test_expect_int("size", avl_size(tree, &size), OK);
  test_expect_size("size == 4", size, 4);
  test_expect_int("min", avl_min(tree, &ptr), OK);
  test_expect_int("min value", *(int*)ptr, 10);
  test_expect_int("remove 10", avl_remove(tree, int_cmp, &values[1]), OK);
  test_expect_int("destroy", avl_destroy(&tree), OK);

  return test_result();
}
