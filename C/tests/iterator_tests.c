#include "unit_testing.h"
#include "iterator.h"
#include "array.h"
#include "list.h"
#include "double_container.h"
#include "iterator_functions.h"



static int (^compare_dbl)(const void*, const void*) = ^(const void* lhs_obj, const void* rhs_obj) {
  double lhs = DoubleContainer_get(lhs_obj);
  double rhs = DoubleContainer_get(rhs_obj);
  if( fabs(lhs - rhs) < 0.0001 ) {
    return 0;
  } else if(lhs < rhs) {
    return -1;
  } else {
    return 1;
  }
};

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

static List* build_fixtures_lists() {
  List* list = List_new();
  DoubleContainer* val = DoubleContainer_new(1.0);
  List_append(list, val);

  val = DoubleContainer_new(2.0);
  List_append(list, val);

  val = DoubleContainer_new(3.0);
  List_append(list, val);

  val = DoubleContainer_new(4.0);
  List_append(list, val);

  val = DoubleContainer_new(5.0);
  List_append(list, val);

  val = DoubleContainer_new(6.0);
  List_append(list, val);

  return list;
}

static void free_fixtures_list(List* list) {
  List_free(list, DoubleContainer_free);
}


static void free_fixtures(Array* array) {
  for_each(Array_it(array), ^(void* obj) {
    DoubleContainer* dbl = obj;
    DoubleContainer_free(dbl);
  });

  Array_free(array);
}


static void test_sort() {
  Array* array = build_fixtures();

  sort(Array_it(array), ^(const void* v1_obj, const void* v2_obj) {
    double v1 = DoubleContainer_get(v1_obj);
    double v2 = DoubleContainer_get(v2_obj);

    if(v2 < v1) {
      return -1;
    } else {
      return 1;
    }
  });

  for_each_with_index(Array_it(array), ^(void* obj, size_t index) {
    assert_double_equal(6.0 - (double)index, DoubleContainer_get(obj), 0.0001);
  });

  free_fixtures(array);
}

static void test_sort_lists() {
  List* list = build_fixtures_lists();

  sort(List_it(list), ^(const void* v1_obj, const void* v2_obj) {
    double v1 = DoubleContainer_get(v1_obj);
    double v2 = DoubleContainer_get(v2_obj);

    if(v2 < v1) {
      return -1;
    } else {
      return 1;
    }
  });

  for_each_with_index(List_it(list), ^(void* obj, size_t index) {
    assert_double_equal(6.0 - (double)index, DoubleContainer_get(obj), 0.0001);
  });

  free_fixtures_list(list);
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

static void test_find_last() {
    Array* array = build_fixtures();
  __block size_t count = 0;

  void* elem = find_last(Array_it(array), ^(void* obj) {
    DoubleContainer* dbl = obj;
    count++;
    return DoubleContainer_get(dbl) < 3.5;
  });

  DoubleContainer* result = elem;
  assert_double_equal( 3.0, DoubleContainer_get(result), 0.01);

  assert_equal(4l, count);

  free_fixtures(array);
}

static void test_find_first_with_no_match() {
    Array* array = build_fixtures();
  __block size_t count = 0;

  void* elem = find_first(Array_it(array), ^(__attribute__((unused)) void* obj) {
    count += 1;
    return 0;
  });

  assert_pointers_equal( NULL, elem);

  assert_equal(6l, count);

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

static void test_filter() {
  Array* array = build_fixtures();

  Array* result = filter(Array_it(array), ^(void* elem) {
    return DoubleContainer_get(elem) > 3.0;
  });

  assert_equal(3l, Array_size(result));
  assert_double_equal(4.0, DoubleContainer_get(Array_at(result, 0)), 0.0001);
  assert_double_equal(5.0, DoubleContainer_get(Array_at(result, 1)), 0.0001);
  assert_double_equal(6.0, DoubleContainer_get(Array_at(result, 2)), 0.0001);

  free_fixtures(array);
  Array_free(result);
}

static void test_first() {
  Array* array = build_fixtures();
  assert_double_equal( 1.0, DoubleContainer_get(first(Array_it(array))), 0.00001);
  free_fixtures(array);
}

static void test_last() {
  Array* array = build_fixtures();
  assert_double_equal( 6.0, DoubleContainer_get(last(Array_it(array))), 0.00001);
  free_fixtures(array);
}

static void test_binary_search() {
  Array* array = build_fixtures();

  for(double val = 1.0; val < 6.0; val += 1.0) {
    void* obj = DoubleContainer_new(val);

    size_t index = binsearch(Array_it(array), obj, compare_dbl);
    assert_double_equal(val, DoubleContainer_get(Array_at(array, index)), 0.00001);

    DoubleContainer_free(obj);
  }

  free_fixtures(array);
}


static void test_binary_search_elem_not_present() {
  Array* array = build_fixtures();
  void* obj;
  size_t index;

  obj = DoubleContainer_new(12.0);
  index = binsearch(Array_it(array), obj, compare_dbl);
  assert_equal((size_t) -1l, index);
  DoubleContainer_free(obj);

  obj = DoubleContainer_new(-1.0);
  index = binsearch(Array_it(array), obj, compare_dbl);
  assert_equal((size_t) -1l, index);
  DoubleContainer_free(obj);

  free_fixtures(array);
}

static void test_reverse() {
  Array* reversed = build_fixtures();
  reverse(Array_it(reversed));

  for_each_with_index(Array_it(reversed), ^(void* obj, size_t index) {
    assert_double_equal(6.0 - (double)index, DoubleContainer_get(obj), 0.0001);
  });

  free_fixtures(reversed);
}

static void test_reverse_lists() {
  List* reversed = build_fixtures_lists();
  reverse(List_it(reversed));

  for_each_with_index(List_it(reversed), ^(void* obj, size_t index) {
    assert_double_equal(6.0 - (double)index, DoubleContainer_get(obj), 0.0001);
  });

  free_fixtures_list(reversed);
}

int main() {
  start_tests("iterators");
  test(test_sort);
  test(test_sort_lists);
  test(test_for_each);
  test(test_for_each_with_index);
  test(test_find_first);
  test(test_find_last);
  test(test_find_first_with_no_match);
  test(test_map);
  test(test_filter);
  test(test_first);
  test(test_last);
  test(test_binary_search);
  test(test_binary_search_elem_not_present);
  test(test_reverse);
  test(test_reverse_lists);
  end_tests();

  return 0;
}
