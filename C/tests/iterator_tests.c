#include "unit_testing.h"
#include "iterator.h"
#include "array.h"
#include "double_container.h"
#include "iterator_functions.h"

static Array* build_fixtures() {
  Array* array = Array_new(10);
  DoubleContainer* val = DoubleContainer_new(1.0);
  Array_add(array, val);

  val = DoubleContainer_new(2.0);
  Array_add(array, val);

  val = DoubleContainer_new(3.0);
  Array_add(array, val);

  val = DoubleContainer_new(4.0);
  Array_add(array, val);

  val = DoubleContainer_new(5.0);
  Array_add(array, val);

  val = DoubleContainer_new(6.0);
  Array_add(array, val);

  return array;
}

static void free_fixtures(Array* array) {
  for_each(Array_it(array), ^(void* obj) {
    DoubleContainer* dbl = obj;
    DoubleContainer_free(dbl);
  });

  Array_free(array);
}

static void test_for_each() {
  Array* array = build_fixtures();
  __block size_t index = 0;
  __block size_t count = 0;

  for_each(Array_it(array), ^(void* obj) {
    void* expected = Array_at(array, index++);
    assert_pointers_equal(expected, obj);
    count++;
  });

  assert_equal(6l, count);

  free_fixtures(array);
}

static void test_for_each_with_index() {
  Array* array = build_fixtures();
  __block size_t cur_index = 0;
  __block size_t count = 0;

  for_each_with_index(Array_it(array), ^(void* obj, size_t index) {
    void* expected = Array_at(array, index);

    assert_equal(cur_index++, index);
    assert_pointers_equal(expected, obj);
    count++;
  });

  assert_equal(6l, count);

  free_fixtures(array);
}



static void test_find_first() {
    Array* array = build_fixtures();
  __block size_t count = 0;

  void* elem = find_first(Array_it(array), ^(void* obj) {
    DoubleContainer* dbl = obj;
    count++;
    return DoubleContainer_get(dbl) > 3.5;
  });

  DoubleContainer* result = elem;
  assert_double_equal( 4.0, DoubleContainer_get(result), 0.01);

  assert_equal(4l, count);

  free_fixtures(array);
}

static void test_map() {
  Array* array = build_fixtures();
  Array* mapped = map(Array_it(array), ^(void* obj) {
    DoubleContainer* dbl = obj;
    return (void*) DoubleContainer_new( DoubleContainer_get(dbl) * 2);
  });

  for_each_with_index(Array_it(array), ^(void* obj, size_t index) {
    DoubleContainer* elem = obj;
    DoubleContainer* other = Array_at(mapped, index);

    assert_double_equal( DoubleContainer_get(elem) * 2, DoubleContainer_get(other), 0.0001);
  });

  free_fixtures(array);
  free_fixtures(mapped);
}

int main() {
  start_tests("iterators");
  test(test_for_each);
  test(test_for_each_with_index);
  test(test_find_first);
  test(test_map);


  end_tests();
  return 0;
}
