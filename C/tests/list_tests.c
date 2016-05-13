#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "unit_testing.h"

static List build_fixtures() {
  List list = List_new();
  list = List_insert(list, "1");
  list = List_insert(list, "81");
  list = List_insert(list, "11");
  list = List_insert(list, "4");
  list = List_insert(list, "6");
  list = List_insert(list, "12");
  list = List_insert(list, "21");
  list = List_insert(list, "3");

  return list;
}

static void test_list_creation() {
  List list = List_new();
  assert_null((void*)list);
  assert_equal((unsigned long)List_length(list), (long)0);
}

static void test_list_insertion() {
  List list = List_new();
  list = List_insert(list, "1");
  assert_not_null(list);
  assert_true(!strcmp(List_get(list), "1"));
  assert_equal((unsigned long) List_length(list), 1l);

  list = List_insert(list, "2");
  assert_not_null(list);
  assert_not_null(List_next(list));
  assert_true(strcmp(List_get(list), "2")==0);
  assert_true(strcmp(List_get(List_next(list)), "1")==0);
  assert_equal((unsigned long) List_length(list), 2l);
}

static void test_list_find_wb() {
  List list = build_fixtures();

  List* list_elem = List_find_wb(&list,  ^int(const void* elem) {
    return strcmp(elem, "4");
  });

  assert_not_null(list_elem);
  assert_not_null(*list_elem);

  assert_true(strcmp(List_get(*list_elem), "4")==0);
}

static int select_str(const char* str) {
  return strcmp(str, "4");
}

static void test_list_find() {
  List list = build_fixtures();

  List* list_elem = List_find(&list, (int (*)(const void*)) select_str);

  assert_not_null(list_elem);
  assert_not_null(*list_elem);

  assert_true(strcmp(List_get(*list_elem), "4")==0);

}

static long delete_elem(void* elem) {
  static long count = 0;

  if(elem) {
    count++;
  }

  return count;
}

static void test_list_delete_node() {
  List list = build_fixtures();

  List_delete_node(&list);
  assert_true(!strcmp(List_get(list), "21") );
}

static void test_list_free_with_delete_elem() {
  List list = build_fixtures();

  List_free(list, (void (*)(void*)) delete_elem);
  assert_equal(delete_elem(NULL), 8l);
}

int main() {
  start_tests("lists");
  test(test_list_creation);
  test(test_list_insertion);
  test(test_list_find_wb);
  test(test_list_find);
  test(test_list_delete_node);
  test(test_list_free_with_delete_elem);
  end_tests();

  return 0;
}