#include "array.h"
#include "unit_testing.h"

static void test_array_creation() {
  Array array = Array_new(10, sizeof(int));
  assert_equal( Array_size(array), 0l);
  assert_equal( Array_capacity(array), 10l);
  assert_not_null( Array_carray(array) );
}

static void* from_int(int elem) {
  static int elem_p;
  elem_p = elem;
  return &elem_p;
}


static int to_int(void* p) {
  long int l_elem = (long int) p;
  return (int) (l_elem & 0xffffffff);
}

static void test_array_set_and_at() {
  Array array = Array_new(10, sizeof(int));
  Array_set_size(array, 10);

  Array_set(array, 2, from_int(3));
  Array_set(array, 3, from_int(4));
  Array_set(array, 4, from_int(5));


  assert_equal(3l, (long) to_int(Array_at(array, 2l)));
  assert_equal(4l, (long) to_int(Array_at(array, 3l)));
  assert_equal(5l, (long) to_int(Array_at(array, 4l)));
}


static void test_array_iterator() {
  Array array = Array_new(10, sizeof(int));
  Array_set_size(array, 5);
  Array_set(array, 0, from_int(1));
  Array_set(array, 1, from_int(2));
  Array_set(array, 2, from_int(3));
  Array_set(array, 3, from_int(4));
  Array_set(array, 4, from_int(5));

  ArrayIterator it = ArrayIterator_new(array);
  int count = 1;
  while(!ArrayIterator_end(it)) {
    int elem = to_int(ArrayIterator_get(it));
    assert_equal((long) count, (long) elem);
    count += 1;
    ArrayIterator_next(it);
  }
}


int main() {
  start_tests("array");

  test(test_array_creation);
  test(test_array_set_and_at);
  // test(test_array_set_out_of_bound_index);
  // test(test_array_add);
  // test(test_array_insert);
  // test(test_array_remove);
  test(test_array_iterator);

  end_tests();


  return 0;
}
