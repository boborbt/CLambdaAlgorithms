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
  assert_equal((unsigned long) List_length(list), (long)1);

  list = List_insert(list, "2");
  assert_not_null(list);
  assert_not_null(List_next(list));
  assert_true(strcmp(List_get(list), "2")==0);
  assert_true(strcmp(List_get(List_next(list)), "1")==0);
  assert_equal((unsigned long) List_length(list), (long)2);
}

static void test_list_find_wb() {
  List list = build_fixtures();

  void* elem = List_find_wb(list, "4", ^int(const void* key1, const void* key2) {
    return strcmp(key1, key2);
  });

  assert_not_null(elem);

  assert_true(strcmp(elem, "4")==0);
}

static void test_list_find() {
  List list = build_fixtures();

  void* elem = List_find(list, "4", (int (*)(const void*, const void*)) strcmp);

  assert_not_null(elem);

  assert_true(strcmp(elem, "4")==0);

}



int main() {
  start_tests("lists");
  test(test_list_creation);
  test(test_list_insertion);
  test(test_list_find_wb);
  test(test_list_find);
  end_tests();

  return 0;
}
