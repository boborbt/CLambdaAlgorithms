#include "array_alt.h"
#include "unit_testing.h"
#include "errors.h"
#include "iterator_functions.h"
#include "mem.h"

typedef struct _TestRecord* TestRecord;

struct _TestRecord{
  int field1;
  int field2;
};


static void* from_int(int elem) {
  static int elem_p;
  elem_p = elem;
  return &elem_p;
}


static int to_int(const void* p) {
  return *(const int*) p;
}

static ArrayAlt build_fixtures() {
  ArrayAlt array = ArrayAlt_new(10, sizeof(int));
  ArrayAlt_set_size(array, 5);
  ArrayAlt_set(array, 0, from_int(1));
  ArrayAlt_set(array, 1, from_int(2));
  ArrayAlt_set(array, 2, from_int(3));
  ArrayAlt_set(array, 3, from_int(4));
  ArrayAlt_set(array, 4, from_int(5));

  return array;
}

static ArrayAlt build_fixtures2() {
  ArrayAlt array = ArrayAlt_new(10, sizeof(int));
  ArrayAlt_set_size(array, 5);
  ArrayAlt_set(array, 0, from_int(5));
  ArrayAlt_set(array, 1, from_int(2));
  ArrayAlt_set(array, 2, from_int(1));
  ArrayAlt_set(array, 3, from_int(3));
  ArrayAlt_set(array, 4, from_int(4));

  return array;
}


static void test_array_creation() {
  ArrayAlt array = ArrayAlt_new(10, sizeof(int));
  assert_equal( ArrayAlt_size(array), 0l);
  assert_equal( ArrayAlt_capacity(array), 10l);
  assert_not_null( ArrayAlt_carray(array) );

  ArrayAlt_free(array);
}



static void test_array_set_and_at() {
  ArrayAlt array = ArrayAlt_new(10, sizeof(int));
  ArrayAlt_set_size(array, 10);

  ArrayAlt_set(array, 2, from_int(3));
  ArrayAlt_set(array, 3, from_int(4));
  ArrayAlt_set(array, 4, from_int(5));


  assert_equal(3l, (long) to_int(ArrayAlt_at(array, 2l)));
  assert_equal(4l, (long) to_int(ArrayAlt_at(array, 3l)));
  assert_equal(5l, (long) to_int(ArrayAlt_at(array, 4l)));

  ArrayAlt_free(array);
}

static void test_array_set_out_of_bound_index() {
  ArrayAlt array = build_fixtures();
  assert_exits_with_code(ArrayAlt_set(array, 11, from_int(3)), ERROR_INDEX_OUT_OF_BOUND);
  ArrayAlt_free(array);
}


static void test_array_iterator() {
  ArrayAlt array = build_fixtures();

  ArrayAltIterator it = ArrayAltIterator_new(array);
  int count = 1;
  while(!ArrayAltIterator_end(it)) {
    int elem = to_int(ArrayAltIterator_get(it));
    assert_equal((long) count, (long) elem);
    count += 1;
    ArrayAltIterator_next(it);
  }
  ArrayAltIterator_free(it);

  ArrayAlt_free(array);
}

static void test_array_add() {
  ArrayAlt array = build_fixtures();
  ArrayAlt_add(array, from_int(11));

  assert_equal(6l, ArrayAlt_size(array));
  assert_equal(10l, ArrayAlt_capacity(array));
  assert_equal(11l,  (long)to_int(ArrayAlt_at(array, 5)));
  ArrayAlt_free(array);
}


static void test_array_add_with_new_capacity() {
  ArrayAlt array = build_fixtures();
  ArrayAlt_add(array, from_int(11));
  ArrayAlt_add(array, from_int(12));
  ArrayAlt_add(array, from_int(13));
  ArrayAlt_add(array, from_int(14));
  ArrayAlt_add(array, from_int(15));
  ArrayAlt_add(array, from_int(16));

  assert_equal(11l, ArrayAlt_size(array));
  assert_equal(20l, ArrayAlt_capacity(array));

  ArrayAlt_free(array);
}

static void test_array_insert_at_0() {
  ArrayAlt array = build_fixtures();
  ArrayAlt_insert(array, 0l, from_int(11));

  assert_equal(6l, ArrayAlt_size(array));
  assert_equal(10l, ArrayAlt_capacity(array));
  assert_equal(11l,  (long)to_int(ArrayAlt_at(array, 0l)));
  assert_equal(1l,  (long)to_int(ArrayAlt_at(array, 1l)));
  assert_equal(2l,  (long)to_int(ArrayAlt_at(array, 2l)));
  assert_equal(3l,  (long)to_int(ArrayAlt_at(array, 3l)));
  assert_equal(4l,  (long)to_int(ArrayAlt_at(array, 4l)));
  assert_equal(5l,  (long)to_int(ArrayAlt_at(array, 5l)));

  ArrayAlt_free(array);
}


static void test_array_insert_at_middle() {
  ArrayAlt array = build_fixtures();
  ArrayAlt_insert(array, 3l, from_int(11));

  assert_equal(6l, ArrayAlt_size(array));
  assert_equal(10l, ArrayAlt_capacity(array));
  assert_equal(11l,  (long)to_int(ArrayAlt_at(array, 3l)));
  assert_equal(3l,  (long)to_int(ArrayAlt_at(array, 2l)));
  assert_equal(4l,  (long)to_int(ArrayAlt_at(array, 4l)));

  ArrayAlt_free(array);
}

static void test_array_insert_at_end() {
  ArrayAlt array = build_fixtures();
  ArrayAlt_insert(array, 5l, from_int(11));

  assert_equal(6l, ArrayAlt_size(array));
  assert_equal(10l, ArrayAlt_capacity(array));

  assert_equal(11l,  (long)to_int(ArrayAlt_at(array, 5l)));
  assert_equal(5l,  (long)to_int(ArrayAlt_at(array, 4l)));

  ArrayAlt_free(array);
}

static void test_array_remove_at_0() {
  ArrayAlt array = build_fixtures();

  ArrayAlt_remove(array, 0l);

  assert_equal(4l, ArrayAlt_size(array));
  assert_equal(10l, ArrayAlt_capacity(array));

  assert_equal(2l,  (long)to_int(ArrayAlt_at(array, 0l)));
  assert_equal(3l,  (long)to_int(ArrayAlt_at(array, 1l)));
  assert_equal(4l,  (long)to_int(ArrayAlt_at(array, 2l)));
  assert_equal(5l,  (long)to_int(ArrayAlt_at(array, 3l)));

  ArrayAlt_free(array);
}

static void test_array_remove_at_middle() {
  ArrayAlt array = build_fixtures();
  ArrayAlt_remove(array, 3l);

  assert_equal(4l, ArrayAlt_size(array));
  assert_equal(10l, ArrayAlt_capacity(array));

  assert_equal(3l,  (long)to_int(ArrayAlt_at(array, 2l)));
  assert_equal(5l,  (long)to_int(ArrayAlt_at(array, 3l)));

  ArrayAlt_free(array);
}

static void test_array_remove_at_end() {
  ArrayAlt array = build_fixtures();
  ArrayAlt_remove(array, 4l);

  assert_equal(4l, ArrayAlt_size(array));
  assert_equal(10l, ArrayAlt_capacity(array));

  assert_equal(4l,  (long)to_int(ArrayAlt_at(array, 3l)));

  ArrayAlt_free(array);
}


static void test_array_foreach() {
  ArrayAlt array = build_fixtures();
  __block size_t count = 1;

  for_each(ArrayAlt_it(array), ^(void* elem) {
    assert_equal( count, (unsigned long) to_int(elem));
    count+=1;
  });

  ArrayAlt_free(array);
}

static void test_array_foreach_with_index() {
  ArrayAlt array = build_fixtures();

  for_each_with_index( ArrayAlt_it(array),  ^(void* elem, size_t index) {
    assert_equal( index+1, (unsigned long) to_int(elem));
  });

  ArrayAlt_free(array);
}

static int compare_ints(const void* e1, const void* e2) {
  int i1 = to_int(e1);
  int i2 = to_int(e2);
  return i1-i2;
}

static void test_array_sort() {
  ArrayAlt array = build_fixtures2();
  ArrayAlt_sort(array, compare_ints);
  for_each_with_index( ArrayAlt_it(array),  ^(void* elem, size_t index) {
    assert_equal( (unsigned long) to_int(elem), index + 1);
  });

  ArrayAlt_free(array);
}

static void test_array_dup() {
  ArrayAlt array = build_fixtures();
  ArrayAlt array_dup = ArrayAlt_dup(array);

  for_each_with_index( ArrayAlt_it(array),  ^(void* elem, size_t index) {
    assert_equal32( to_int(elem), to_int(ArrayAlt_at(array_dup, index)));
  });

  assert_pointers_not_equal( ArrayAlt_carray(array), ArrayAlt_carray(array_dup) );

  ArrayAlt_free(array);
  ArrayAlt_free(array_dup);
}


static void test_array_add_records() {
  ArrayAlt array = ArrayAlt_new(10, sizeof(TestRecord));

  TestRecord tr1 = (TestRecord) Mem_alloc(sizeof(struct _TestRecord));
  tr1->field1 = 1;
  tr1->field2 = 2;

  TestRecord tr2 = (TestRecord) Mem_alloc(sizeof(struct _TestRecord));
  tr2->field1 = 3;
  tr2->field2 = 4;

  TestRecord tr3 = (TestRecord) Mem_alloc(sizeof(struct _TestRecord));
  tr3->field1 = 5;
  tr3->field2 = 6;

  ArrayAlt_add(array, &tr1);
  ArrayAlt_add(array, &tr2);
  ArrayAlt_add(array, &tr3);

  TestRecord tr = *(TestRecord*)ArrayAlt_at(array, 0);
  assert_pointers_equal((void*)tr, (void*)tr1);
  assert_equal32(tr->field1, 1);
  assert_equal32(tr->field2, 2);

  tr = *(TestRecord*) ArrayAlt_at(array, 1);
  assert_pointers_equal((void*)tr, (void*)tr2);
  assert_equal32(tr->field1, 3);
  assert_equal32(tr->field2, 4);

  tr = *(TestRecord*) ArrayAlt_at(array, 2);
  assert_pointers_equal((void*)tr, (void*)tr3);
  assert_equal32(tr->field1, 5);
  assert_equal32(tr->field2, 6);

  for_each(ArrayAlt_it(array), ^(void* elem) {
    TestRecord tmp = *(TestRecord*) elem;
    Mem_free(tmp);
  });

  ArrayAlt_free(array);
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

  test(test_array_add_records);

  end_tests();

  return 0;
}
