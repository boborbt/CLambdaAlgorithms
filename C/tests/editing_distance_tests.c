#include "unit_testing.h"
#include "editing_distance.h"
#include <limits.h>

static void test_empty_strings() {
  assert_equal(editing_distance("","", (unsigned long) -2), 0l);
}

static void test_first_string_empty() {
  assert_equal(editing_distance("", "test", (unsigned long) -2), 4l);
}

static void test_second_string_empty() {
  assert_equal(editing_distance("test","", (unsigned long) -2), 4l);
}

static void test_single_addition() {
  assert_equal(editing_distance("test","tst", (unsigned long) -2), 1l);
}

static void test_single_deletion() {
  assert_equal(editing_distance("tst", "test", (unsigned long) -2), 1l);
}

static void test_matching_strings() {
  assert_equal(editing_distance("test", "test", (unsigned long) -2), 0l);
}

static void test_complex_editing() {
  assert_equal(editing_distance("impresario", "reimpresta", (unsigned long) -2), 6l);
}

static void test_scuola() {
  assert_equal(editing_distance("scuola", "squola", (unsigned long) -2), 1l);
  assert_equal(editing_distance("scuola", "squola", (unsigned long) 5), 1l);
  assert_equal(editing_distance("scuola", "squola", (unsigned long) 4), 1l);
  assert_equal(editing_distance("scuola", "squola", (unsigned long) 3), 1l);
  assert_equal(editing_distance("scuola", "squola", (unsigned long) 2), 1l);
  assert_equal(editing_distance("scuola", "squola", (unsigned long) 1), 1l);
  assert_equal(editing_distance("scuola", "squola", (unsigned long) 0), 1l);
}

static void test_scuola2() {
  assert_equal(editing_distance("scuola", "stalle", (unsigned long) -2), 4l);
  assert_equal(editing_distance("scuola", "stalle", (unsigned long) 5), 4l);
  assert_equal(editing_distance("scuola", "stalle", (unsigned long) 4), 4l);
  assert_equal(editing_distance("scuola", "stalle", (unsigned long) 3), ULONG_MAX);
  assert_equal(editing_distance("scuola", "stalle", (unsigned long) 2), ULONG_MAX);
  assert_equal(editing_distance("scuola", "stalle", (unsigned long) 1), ULONG_MAX);
  assert_equal(editing_distance("scuola", "stalle", (unsigned long) 0), ULONG_MAX);
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
