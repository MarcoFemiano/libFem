#include "LinkedList.h"
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
  LinkedList list = NULL;
  int one = 1;
  int two = 2;
  int three = 3;
  int four = 4;
  int absent = 99;
  int* result = NULL;

  list = linkedList_create();
  test_expect_true("create list", list != NULL);
  test_expect_int("empty after create", linkedList_isEmpty(list), true);
  test_expect_size("size after create", linkedList_size(list), 0);

  test_expect_int("add head 2", linkedList_add_head(list, &two), LINKED_LIST_OK);
  test_expect_int("add head 1", linkedList_add_head(list, &one), LINKED_LIST_OK);
  test_expect_int("add tail 4", linkedList_add_tail(list, &four), LINKED_LIST_OK);
  test_expect_int("add at 3", linkedList_add_at(list, &three, 2), LINKED_LIST_OK);
  test_expect_size("size after inserts", linkedList_size(list), 4);
  test_expect_int("not empty", linkedList_isEmpty(list), false);

  result = linkedList_search_index(list, 0);
  test_expect_true("index 0 not null", result != NULL);
  test_expect_int("index 0 value", *result, 1);

  result = linkedList_search_index(list, 2);
  test_expect_true("index 2 not null", result != NULL);
  test_expect_int("index 2 value", *result, 3);

  result = linkedList_search(list, int_cmp, &three);
  test_expect_true("search 3 not null", result != NULL);
  test_expect_int("search 3 value", *result, 3);

  test_expect_true("search absent", linkedList_search(list, int_cmp, &absent) == NULL);
  test_expect_int("remove value 3", linkedList_remove_val(list, int_cmp, &three), LINKED_LIST_OK);
  test_expect_size("size after remove value", linkedList_size(list), 3);
  test_expect_true("search removed", linkedList_search(list, int_cmp, &three) == NULL);

  test_expect_int("remove at 1", linkedList_remove_at(list, 1), LINKED_LIST_OK);
  result = linkedList_search_index(list, 1);
  test_expect_true("index 1 after remove not null", result != NULL);
  test_expect_int("index 1 after remove", *result, 4);

  test_expect_int("remove head", linkedList_remove_head(list), LINKED_LIST_OK);
  test_expect_int("remove tail", linkedList_remove_tail(list), LINKED_LIST_OK);
  test_expect_int("empty after removes", linkedList_isEmpty(list), true);
  test_expect_int("remove empty", linkedList_remove_tail(list), LINKED_LIST_ERROR_NULL_POINTER);

  test_expect_int("add after empty", linkedList_add_tail(list, &one), LINKED_LIST_OK);
  test_expect_int("clear", linkedList_clear(list), LINKED_LIST_OK);
  test_expect_int("empty after clear", linkedList_isEmpty(list), true);
  test_expect_int("destroy", linkedList_destroy(&list), LINKED_LIST_OK);
  test_expect_true("destroy sets NULL", list == NULL);

  return test_result();
}
