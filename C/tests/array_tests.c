#include "array.h"
#include "unit_testing.h"

static void* from_int(int elem) {
  static int elem_p;
  elem_p = elem;
  return &elem_p;
}


static int to_int(const void* p) {
  return *(const int*) p;
}

static Array build_fixtures() {
  Array array = Array_new(10, sizeof(int));
  Array_set_size(array, 5);
  Array_set(array, 0, from_int(1));
  Array_set(array, 1, from_int(2));
  Array_set(array, 2, from_int(3));
  Array_set(array, 3, from_int(4));
  Array_set(array, 4, from_int(5));

  return array;
}

static Array build_fixtures2() {
  Array array = Array_new(10, sizeof(int));
  Array_set_size(array, 5);
  Array_set(array, 0, from_int(5));
  Array_set(array, 1, from_int(2));
  Array_set(array, 2, from_int(1));
  Array_set(array, 3, from_int(3));
  Array_set(array, 4, from_int(4));

  return array;
}


static void test_array_creation() {
  Array array = Array_new(10, sizeof(int));
  assert_equal( Array_size(array), 0l);
  assert_equal( Array_capacity(array), 10l);
  assert_not_null( Array_carray(array) );

  Array_free(array);
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

  Array_free(array);
}

static void test_array_set_out_of_bound_index() {
  Array array = build_fixtures();
  assert_exits_with_code(Array_set(array, 11, from_int(3)), ARRAY_ERROR_OUT_OF_BOUND_INDEX);
  Array_free(array);
}


static void test_array_iterator() {
  Array array = build_fixtures();

  ArrayIterator it = ArrayIterator_new(array);
  int count = 1;
  while(!ArrayIterator_end(it)) {
    int elem = to_int(ArrayIterator_get(it));
    assert_equal((long) count, (long) elem);
    count += 1;
    ArrayIterator_next(it);
  }

  Array_free(array);
}

static void test_array_add() {
  Array array = build_fixtures();
  Array_add(array, from_int(11));

  assert_equal(6l, Array_size(array));
  assert_equal(10l, Array_capacity(array));
  assert_equal(11l,  (long)to_int(Array_at(array, 5)));
  Array_free(array);
}


static void test_array_add_with_new_capacity() {
  Array array = build_fixtures();
  Array_add(array, from_int(11));
  Array_add(array, from_int(12));
  Array_add(array, from_int(13));
  Array_add(array, from_int(14));
  Array_add(array, from_int(15));
  Array_add(array, from_int(16));

  assert_equal(11l, Array_size(array));
  assert_equal(20l, Array_capacity(array));

  Array_free(array);
}

static void test_array_insert_at_0() {
  Array array = build_fixtures();
  Array_insert(array, 0l, from_int(11));

  assert_equal(6l, Array_size(array));
  assert_equal(10l, Array_capacity(array));
  assert_equal(11l,  (long)to_int(Array_at(array, 0l)));
  assert_equal(1l,  (long)to_int(Array_at(array, 1l)));
  assert_equal(2l,  (long)to_int(Array_at(array, 2l)));
  assert_equal(3l,  (long)to_int(Array_at(array, 3l)));
  assert_equal(4l,  (long)to_int(Array_at(array, 4l)));
  assert_equal(5l,  (long)to_int(Array_at(array, 5l)));

  Array_free(array);
}


static void test_array_insert_at_middle() {
  Array array = build_fixtures();
  Array_insert(array, 3l, from_int(11));

  assert_equal(6l, Array_size(array));
  assert_equal(10l, Array_capacity(array));
  assert_equal(11l,  (long)to_int(Array_at(array, 3l)));
  assert_equal(3l,  (long)to_int(Array_at(array, 2l)));
  assert_equal(4l,  (long)to_int(Array_at(array, 4l)));

  Array_free(array);
}

static void test_array_insert_at_end() {
  Array array = build_fixtures();
  Array_insert(array, 5l, from_int(11));

  assert_equal(6l, Array_size(array));
  assert_equal(10l, Array_capacity(array));

  assert_equal(11l,  (long)to_int(Array_at(array, 5l)));
  assert_equal(5l,  (long)to_int(Array_at(array, 4l)));

  Array_free(array);
}

static void test_array_remove_at_0() {
  Array array = build_fixtures();

  Array_remove(array, 0l);

  assert_equal(4l, Array_size(array));
  assert_equal(10l, Array_capacity(array));

  assert_equal(2l,  (long)to_int(Array_at(array, 0l)));
  assert_equal(3l,  (long)to_int(Array_at(array, 1l)));
  assert_equal(4l,  (long)to_int(Array_at(array, 2l)));
  assert_equal(5l,  (long)to_int(Array_at(array, 3l)));

  Array_free(array);
}

static void test_array_remove_at_middle() {
  Array array = build_fixtures();
  Array_remove(array, 3l);

  assert_equal(4l, Array_size(array));
  assert_equal(10l, Array_capacity(array));

  assert_equal(3l,  (long)to_int(Array_at(array, 2l)));
  assert_equal(5l,  (long)to_int(Array_at(array, 3l)));

  Array_free(array);
}

static void test_array_remove_at_end() {
  Array array = build_fixtures();
  Array_remove(array, 4l);

  assert_equal(4l, Array_size(array));
  assert_equal(10l, Array_capacity(array));

  assert_equal(4l,  (long)to_int(Array_at(array, 3l)));

  Array_free(array);
}


static void test_array_foreach() {
  Array array = build_fixtures();
  __block size_t count = 1;

  foreach_array_elem(array, ^(void* elem) {
    assert_equal( count, (unsigned long) to_int(elem));
    count+=1;
  });

  Array_free(array);
}

static void test_array_foreach_with_index() {
  Array array = build_fixtures();

  foreach_array_elem_with_index(array, ^(void* elem, size_t index) {
    assert_equal( index+1, (unsigned long) to_int(elem));
  });

  Array_free(array);
}

static int compare_ints(const void* e1, const void* e2) {
  int i1 = to_int(e1);
  int i2 = to_int(e2);
  return i1-i2;
}

static void test_array_sort() {
  Array array = build_fixtures2();
  Array_sort(array, compare_ints);
  foreach_array_elem_with_index(array, ^(void* elem, size_t index) {
    assert_equal( (unsigned long) to_int(elem), index + 1);
  });
}

static void test_array_dup() {
  Array array = build_fixtures();
  Array array_dup = Array_dup(array);

  foreach_array_elem_with_index(array, ^(void* elem, size_t index) {
    assert_equal32( to_int(elem), to_int(Array_at(array_dup, index)));
  });

  assert_pointers_not_equal( Array_carray(array), Array_carray(array_dup) );

  Array_free(array);
  Array_free(array_dup);
}


int main() {
  start_tests("array");

  test(test_array_creation);
  test(test_array_set_and_at);
  test(test_array_set_out_of_bound_index);
  test(test_array_add);
  test(test_array_add_with_new_capacity);
  test(test_array_insert_at_0);
  test(test_array_insert_at_middle);
  test(test_array_insert_at_end);
  test(test_array_remove_at_0);
  test(test_array_remove_at_middle);
  test(test_array_remove_at_end);
  test(test_array_sort);

  test(test_array_iterator);
  test(test_array_foreach);
  test(test_array_foreach_with_index);
  test(test_array_dup);

  end_tests();

  return 0;
}