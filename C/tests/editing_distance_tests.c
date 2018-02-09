#include "unit_testing.h"
#include "editing_distance.h"

static void test_empty_strings() {
  assert_equal(editing_distance("",""), 0l);
}

static void test_first_string_empty() {
  assert_equal(editing_distance("", "test"), 4l);
}

static void test_second_string_empty() {
  assert_equal(editing_distance("test",""), 4l);
}

static void test_single_addition() {
  assert_equal(editing_distance("test","tst"), 1l);
}

static void test_single_deletion() {
  assert_equal(editing_distance("tst", "test"), 1l);
}

static void test_matching_strings() {
  assert_equal(editing_distance("test", "test"), 0l);
}

static void test_complex_editing() {
  assert_equal(editing_distance("impresario", "reimpresta"), 6l);
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

  end_tests();
}
