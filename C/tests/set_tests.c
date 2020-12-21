#include "unit_testing.h"
#include "set.h"

static KeyInfo* key_info;

static Set* build_fixtures() {
  key_info = KeyInfo_new(Key_string_compare, Key_string_hash);
  Set* set = Set_new(key_info);
  
  Set_insert(set, "el1");
  Set_insert(set, "el2");
  Set_insert(set, "el3");
  Set_insert(set, "el4");
  Set_insert(set, "el5");
  Set_insert(set, "el6");

  return set;
}

static void free_fixtures(Set* set) {
  Set_free(set);
  KeyInfo_free(key_info);
}

static void test_set_insert() {
  Set* set = build_fixtures();

  assert_equal(6L, Set_size(set));
  assert_false(Set_contains(set, "el7"));
  Set_insert(set, "el7");
  assert_true(Set_contains(set, "el7"));
  assert_equal(7L, Set_size(set));

  free_fixtures(set);
}

static void test_set_union() {
  Set* set1 = build_fixtures();
  Set* set2 = Set_new(Set_key_info(set1));
  Set_insert(set2, "el5");
  Set_insert(set2, "el1");
  Set_insert(set2, "el8");
  Set_insert(set2, "el11");

  Set* expected = Set_new(Set_key_info(set1));
  Set_insert(expected, "el1");
  Set_insert(expected, "el2");
  Set_insert(expected, "el3");
  Set_insert(expected, "el4");
  Set_insert(expected, "el5");
  Set_insert(expected, "el6");
  Set_insert(expected, "el8");
  Set_insert(expected, "el11");

  Set* result = Set_union(set1, set2);
  assert_true(Set_equals(expected, result));

  Set_free(set2);
  Set_free(result);
  Set_free(expected);

  free_fixtures(set1);
}

static void test_set_inplace_union() {
  Set *set1 = build_fixtures();

  Set *set2 = Set_new(Set_key_info(set1));
  Set_insert(set2, "el5");
  Set_insert(set2, "el1");
  Set_insert(set2, "el8");
  Set_insert(set2, "el11");

  Set *expected = Set_new(Set_key_info(set1));
  Set_insert(expected, "el1");
  Set_insert(expected, "el2");
  Set_insert(expected, "el3");
  Set_insert(expected, "el4");
  Set_insert(expected, "el5");
  Set_insert(expected, "el6");
  Set_insert(expected, "el8");
  Set_insert(expected, "el11");

  Set_inplace_union(set1, set2);
  assert_true(Set_equals(expected, set1));

  Set_free(set2);
  Set_free(expected);

  free_fixtures(set1);
}

static void test_set_size_empty_set() {
  KeyInfo* ki = KeyInfo_new(Key_string_compare, Key_string_hash);
  Set* set = Set_new(ki);
  assert_equal(0l, Set_size(set))
  Set_free(set);
  KeyInfo_free(ki);
}

static void test_set_size() {
  Set *set = build_fixtures();

  assert_equal(6l, Set_size(set));

  free_fixtures(set);
}

static void test_set_contained() {
  Set *set = build_fixtures();

  assert_true( Set_contains(set, "el1"));
  assert_true(Set_contains(set, "el2"));
  assert_true(Set_contains(set, "el3"));
  assert_true(Set_contains(set, "el4"));
  assert_true(Set_contains(set, "el5"));
  assert_true(Set_contains(set, "el6"));

  free_fixtures(set);
}

static void test_set_not_contained() {
  Set *set = build_fixtures();

  assert_false(Set_contains(set, "el7"));

  free_fixtures(set);
}

static void test_set_remove_present() {
  Set *set = build_fixtures();

  assert_true(Set_contains(set, "el4"));
  Set_remove(set, "el4");
  assert_false(Set_contains(set, "el4"));

  free_fixtures(set);
}

static void test_set_remove_not_present() {
  Set *set = build_fixtures();

  Set_remove(set, "el10");
  assert_false(Set_contains(set, "el10"));

  free_fixtures(set);
}

static void test_set_intersection_with_empty_set() {
  KeyInfo* ki = KeyInfo_new(Key_string_compare, Key_string_hash);

  Set *set1 = build_fixtures();
  Set *set2 = Set_new(ki);
  Set *set3 = Set_intersection(set1, set2);

  assert_equal(0l, Set_size(set3));

  free_fixtures(set1);
  Set_free(set2);
  Set_free(set3);
  KeyInfo_free(ki);
}

static void test_set_intersection()
{
  KeyInfo* ki = KeyInfo_new(Key_string_compare, Key_string_hash);

  Set *set1 = build_fixtures();
  Set *set2 = Set_new(ki);
  Set_insert(set2, "el0");
  Set_insert(set2, "el2");
  Set_insert(set2, "el4");
  Set_insert(set2, "el11");
  Set_insert(set2, "el3");

  Set *set3 = Set_intersection(set1, set2);

  assert_equal(3l, Set_size(set3));
  assert_true(Set_contains(set3, "el2"));
  assert_true(Set_contains(set3, "el3"));
  assert_true(Set_contains(set3, "el4"));

  free_fixtures(set1);
  Set_free(set2);
  Set_free(set3);
  KeyInfo_free(ki);
}

static void test_set_inplace_intersection()
{
  KeyInfo *ki = KeyInfo_new(Key_string_compare, Key_string_hash);

  Set *set1 = build_fixtures();
  Set *set2 = Set_new(ki);
  Set_insert(set2, "el0");
  Set_insert(set2, "el2");
  Set_insert(set2, "el4");
  Set_insert(set2, "el11");
  Set_insert(set2, "el3");

  Set_inplace_intersect(set1, set2);

  assert_equal(3l, Set_size(set1));
  assert_true(Set_contains(set1, "el2"));
  assert_true(Set_contains(set1, "el3"));
  assert_true(Set_contains(set1, "el4"));

  free_fixtures(set1);
  Set_free(set2);
  KeyInfo_free(ki);
}

static void test_set_difference_with_superset() {
  KeyInfo *ki = KeyInfo_new(Key_string_compare, Key_string_hash);

  Set *set1 = build_fixtures();
  Set *set2 = Set_new(ki);
  Set_insert(set2, "el0");
  Set_insert(set2, "el1");
  Set_insert(set2, "el2");
  Set_insert(set2, "el3");
  Set_insert(set2, "el4");
  Set_insert(set2, "el5");
  Set_insert(set2, "el6");
  Set_insert(set2, "el7");

  Set *set3 = Set_difference(set1, set2);

  assert_equal(0l, Set_size(set3));

  free_fixtures(set1);
  Set_free(set2);
  Set_free(set3);
  KeyInfo_free(ki);
}

static void test_set_difference_with_subset()
{
  KeyInfo *ki = KeyInfo_new(Key_string_compare, Key_string_hash);

  Set *set1 = build_fixtures();
  Set *set2 = Set_new(ki);
  Set_insert(set2, "el3");
  Set_insert(set2, "el4");
  Set_insert(set2, "el5");

  Set *set3 = Set_difference(set1, set2);

  assert_equal(3l, Set_size(set3));
  assert_true(Set_contains(set3, "el1"));
  assert_true(Set_contains(set3, "el2"));
  assert_true(Set_contains(set3, "el6"));


  free_fixtures(set1);
  Set_free(set2);
  Set_free(set3);
  KeyInfo_free(ki);
}

static void test_set_difference_with_otherset()
{
  KeyInfo *ki = KeyInfo_new(Key_string_compare, Key_string_hash);

  Set *set1 = build_fixtures();
  Set *set2 = Set_new(ki);
  Set_insert(set2, "el0");
  Set_insert(set2, "el3");
  Set_insert(set2, "el5");
  Set_insert(set2, "el7");

  Set *set3 = Set_difference(set1, set2);

  assert_equal(4l, Set_size(set3));
  assert_true(Set_contains(set3, "el1"));
  assert_true(Set_contains(set3, "el2"));
  assert_true(Set_contains(set3, "el4"));
  assert_true(Set_contains(set3, "el6"));

  free_fixtures(set1);
  Set_free(set2);
  Set_free(set3);
  KeyInfo_free(ki);
}

static void test_set_inplace_difference_with_otherset()
{
  KeyInfo *ki = KeyInfo_new(Key_string_compare, Key_string_hash);

  Set *set1 = build_fixtures();
  Set *set2 = Set_new(ki);
  Set_insert(set2, "el0");
  Set_insert(set2, "el3");
  Set_insert(set2, "el5");
  Set_insert(set2, "el7");

  Set_inplace_difference(set1, set2);

  assert_equal(4l, Set_size(set1));
  assert_true(Set_contains(set1, "el1"));
  assert_true(Set_contains(set1, "el2"));
  assert_true(Set_contains(set1, "el4"));
  assert_true(Set_contains(set1, "el6"));

  free_fixtures(set1);
  Set_free(set2);
  KeyInfo_free(ki);
}

int main() {

  start_tests("Sets");
  
  test(test_set_insert);
  test(test_set_union);
  test(test_set_size_empty_set);
  test(test_set_size);
  test(test_set_contained);
  test(test_set_not_contained);
  test(test_set_remove_present);
  test(test_set_remove_not_present);
  test(test_set_intersection_with_empty_set);
  test(test_set_intersection);
  test(test_set_difference_with_superset);
  test(test_set_difference_with_subset);
  test(test_set_difference_with_otherset);
  test(test_set_inplace_union);
  test(test_set_inplace_intersection);
  test(test_set_inplace_difference_with_otherset);

  end_tests();

  return 0;
}
