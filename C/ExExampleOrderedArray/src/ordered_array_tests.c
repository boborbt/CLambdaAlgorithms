#include <assert.h>
#include <string.h>

#include "unity.h"

#include "ordered_array.h"

static OrderedArray* build_fixtures() {
  OrderedArray* array = OrderedArray_new((CompareCallback) strcmp);

  OrderedArray_add(array, "a is for apple");
  OrderedArray_add(array, "b is for ball");
  OrderedArray_add(array, "c is for camper");
  OrderedArray_add(array, "d is for dog");

  return array;
}


static void test_ordered_array_create() {
  OrderedArray* array = OrderedArray_new((CompareCallback) strcmp);

  TEST_ASSERT_NOT_NULL(array);
  TEST_ASSERT_EQUAL_INT(OrderedArray_count(array), 0);
}

static void test_ordered_array_add_beginning() {
  OrderedArray* array = build_fixtures();

  OrderedArray_add(array, "  is for space");

  TEST_ASSERT_EQUAL_STRING( "  is for space", OrderedArray_at(array, 0) );
  TEST_ASSERT_EQUAL_STRING( "a is for apple", OrderedArray_at(array, 1) );

  OrderedArray_free(array);
}

static void test_ordered_array_add_middle() {
  OrderedArray* array = build_fixtures();

  OrderedArray_add(array, "be is for bee");

  TEST_ASSERT_EQUAL_STRING("b is for ball", OrderedArray_at(array, 1) );
  TEST_ASSERT_EQUAL_STRING("be is for bee", OrderedArray_at(array, 2) );
  TEST_ASSERT_EQUAL_STRING("c is for camper", OrderedArray_at(array, 3) );

  OrderedArray_free(array);
}

static void test_ordered_array_add_end() {
  OrderedArray* array = build_fixtures();

  OrderedArray_add(array, "e is for elefant");

  TEST_ASSERT_EQUAL_STRING("d is for dog", OrderedArray_at(array, 3) );
  TEST_ASSERT_EQUAL_STRING("e is for elefant", OrderedArray_at(array, 4) );

  OrderedArray_free(array);
}

static void test_ordered_array_remove() {
  OrderedArray* array = build_fixtures();

  OrderedArray_remove_at(array, 1);

  TEST_ASSERT_EQUAL_STRING("a is for apple", OrderedArray_at(array, 0) );
  TEST_ASSERT_EQUAL_STRING("c is for camper", OrderedArray_at(array, 1) );

  OrderedArray_free(array);
}

int main() {

  UNITY_BEGIN();

  RUN_TEST(test_ordered_array_create);
  RUN_TEST(test_ordered_array_add_beginning);
  RUN_TEST(test_ordered_array_add_middle);
  RUN_TEST(test_ordered_array_add_end);
  RUN_TEST(test_ordered_array_remove);

  return UNITY_END();
}
