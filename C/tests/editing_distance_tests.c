#include "unit_testing.h"
#include "editing_distance.h"
#include <limits.h>

static void test_empty_strings() {
  assert_equal(editing_distance("","", LONG_MAX), 0l);
}

static void test_first_string_empty() {
  assert_equal(editing_distance("", "test", LONG_MAX), 4l);
}

static void test_second_string_empty() {
  assert_equal(editing_distance("test","", LONG_MAX), 4l);
}

static void test_single_addition() {
  assert_equal(editing_distance("test","tst", LONG_MAX), 1l);
}

static void test_single_deletion() {
  assert_equal(editing_distance("tst", "test", LONG_MAX), 1l);
}

static void test_matching_strings() {
  assert_equal(editing_distance("test", "test", LONG_MAX), 0l);
}

static void test_complex_editing() {
  assert_equal(editing_distance("impresario", "reimpresta", LONG_MAX), 6l);
}

static void test_scuola() {
  assert_equal(editing_distance("scuola", "squola", LONG_MAX), 1l);
  assert_equal(editing_distance("scuola", "squola", 5l), 1l);
  assert_equal(editing_distance("scuola", "squola", 4l), 1l);
  assert_equal(editing_distance("scuola", "squola", 3l), 1l);
  assert_equal(editing_distance("scuola", "squola", 2l), 1l);
  assert_equal(editing_distance("scuola", "squola", 1l), 1l);
  assert_equal(editing_distance("scuola", "squola", 0l), (long) LONG_MAX);
}

static void test_scuola2() {
  assert_equal(editing_distance("scuola", "stalle", LONG_MAX), 4l);
  assert_equal(editing_distance("scuola", "stalle", 5l), 4l);
  assert_equal(editing_distance("scuola", "stalle", 4l), 4l);
  assert_equal(editing_distance("scuola", "stalle", 3l), (long)LONG_MAX);
  assert_equal(editing_distance("scuola", "stalle", 2l), (long)LONG_MAX);
  assert_equal(editing_distance("scuola", "stalle", 1l), (long)LONG_MAX);
  assert_equal(editing_distance("scuola", "stalle", 0l), (long)LONG_MAX);
}

int main() {
  start_tests("editing distance");
  test(test_empty_strings);
  test(test_first_string_empty);
  test(test_second_string_empty);
  test(test_single_addition);
  test(test_single_deletion);
  test(test_matching_strings);
  test(test_complex_editing);
  test(test_scuola);
  test(test_scuola2);

  end_tests();
}
