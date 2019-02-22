#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "unit_testing.h"
#include "iterator_functions.h"
#include "macros.h"

static List* build_fixtures() {
  List* list = List_new();
  List_insert(list, "1");
  List_insert(list, "81");
  List_insert(list, "11");
  List_insert(list, "4");
  List_insert(list, "6");
  List_insert(list, "12");
  List_insert(list, "21");
  List_insert(list, "3");

  return list;
}

static void test_list_creation() {
  List* list = List_new();
  assert_equal((unsigned long)List_size(list), (long)0);
  List_free(list, NULL);
}

static void test_list_insertion() {
  List* list = List_new();
  List_insert(list, "1");
  assert_not_null(list);
  assert_equal(1l, List_size(list));
  assert_true(!strcmp(List_get_head(list), "1"));

  List_insert(list, "2");
  assert_equal(2l, List_size(list));
  ListIterator* it = ListIterator_new(list);

  assert_true(strcmp(ListIterator_get(it), "2")==0);
  ListIterator_next(it);

  assert_true(strcmp(ListIterator_get(it), "1")==0);
  ListIterator_next(it);

  assert_true(ListIterator_end(it));
  ListIterator_free(it);
  List_free(list, NULL);
}

static void test_list_find_wb() {
  List* list = build_fixtures();

  ListNode* list_elem = List_find_wb(list,  ^int(const void* elem) {
    return strcmp(elem, "4")==0;
  });

  assert_not_null(list_elem);

  assert_true(strcmp(ListNode_get(list, list_elem), "4"));
  List_free(list, NULL);
}

static int select_str(const char* str) {
  return strcmp(str, "4");
}

static void test_list_find() {
  List* list = build_fixtures();

  ListNode* list_elem = List_find(list, (int (*)(const void*)) select_str);

  assert_not_null(list_elem);

  assert_true(strcmp(ListNode_get(list, list_elem), "4")==0);
  List_free(list, NULL);
}

static long delete_elem(void* elem) {
  static long count = 0;

  if(elem) {
    count++;
  }

  return count;
}

static void test_list_delete_node() {
  List* list = build_fixtures();
  ListNode* it = List_head(list);

  List_delete_node(list, it);

  it = List_head(list);

  assert_true(!strcmp(ListNode_get(list, it), "21") );
  assert_equal(7l, List_size(list));

  List_free(list, NULL);
}

static void test_list_free_with_delete_elem() {
  List* list = build_fixtures();

  List_free(list, (void (*)(void*)) delete_elem);
  assert_equal(delete_elem(NULL), 8l);
}

static void test_list_foreach() {
  List* list = build_fixtures();
  __block size_t count = 0;

  for_each(List_it(list), ^(UNUSED(void* elem)) {
    count += 1;
  });

  assert_equal(count, List_size(list));

  List_free(list, NULL);
}

int main() {
  start_tests("lists");
  test(test_list_creation);
  test(test_list_insertion);
  test(test_list_find_wb);
  test(test_list_find);
  test(test_list_delete_node);
  test(test_list_free_with_delete_elem);
  test(test_list_foreach);
  // test(test_list_foreach_reverse);
  end_tests();

  return 0;
}
