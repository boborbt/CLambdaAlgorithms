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


int main() {

  start_tests("Sets");
  
  test(test_set_insert);
  test(test_set_union);

  end_tests();


  return 0;
}
