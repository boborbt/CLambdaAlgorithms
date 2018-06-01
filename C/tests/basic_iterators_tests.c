#include "basic_iterators.h"
#include "unit_testing.h"
#include "iterator_functions.h"
#include "mem.h"

static void test_int_iterator() {
  __block unsigned long count = 10l;

  for_each(Number_it(10), ^(void* obj){
    unsigned long current = *(unsigned int*) obj;
    assert_equal(10l, count + current);
    count--;
  });

  assert_equal(0l, count);
}

static void test_file_iterator() {
  char fname_template[] = "/tmp/basic_iterators_tempfile.XXXXXX";
  char* fname = mktemp(fname_template);
  FILE* file = fopen(fname, "w");
  char* file_contents[] = {"test", "of", "file iterator", " with for_each"};
  __block Array* array_with_fcontents = Array_new_by_copying_carray(file_contents, 4);

  fprintf(file, "test\nof\nfile iterator\n with for_each\n");
  fclose(file);

  for_each_with_index(TextFile_it(fname, '\n'), ^(void* obj, size_t index) {
    assert_string_equal(Array_at(array_with_fcontents, index), (const char*) obj);
  });

  Array_free(array_with_fcontents);
}

static void test_char_iterator_reverse() {
  char* string = Mem_strdup("boon");
  char* reversed = "noob";

  reverse(Char_it(string));

  for_each_with_index(Char_it(string), ^(void* obj, size_t index) {
    assert_char_equal(reversed[index], CH(obj));
  });

  Mem_free(string);
}

static void test_char_iterator_sort() {
  char* string = Mem_strdup("bobo");
  char* sorted = "bboo";

  sort(Char_it(string), ^(const void* obj1, const void* obj2) {
    char c1 = *(const char*)obj1;
    char c2 = *(const char*)obj2;
    if(c1<c2) {
      return -1;
    }
    if(c1>c2) {
      return 1;
    }
    return 0;
  });

  assert_string_equal(string, sorted);

  Mem_free(string);
}

typedef struct {
  int i;
  int j;
} CArrayTestStruct;

static void test_carray_iterator() {
  CArrayTestStruct array[5];
  for(int i=0; i<5; ++i) {
    array[i].i = i;
    array[i].j = -i;
  }

  for_each_with_index(CArray_it(array, 5, sizeof(CArrayTestStruct)), ^(void* obj, size_t index) {
    CArrayTestStruct* elem = (CArrayTestStruct*) obj;
    assert_equal((long) index, (long) elem->i);
    assert_equal((long)-index, (long) elem->j);
  });
}

static void test_carray_iterator_reverse() {
  CArrayTestStruct array[5];
  for(int i=0; i<5; ++i) {
    array[i].i = i;
    array[i].j = -i;
  }

  reverse(CArray_it(array, 5, sizeof(CArrayTestStruct)));

  for_each_with_index(CArray_it(array, 5, sizeof(CArrayTestStruct)), ^(void* obj, size_t index) {
    CArrayTestStruct* elem = (CArrayTestStruct*) obj;
    assert_equal((long)(4 - index), (long) elem->i);
    assert_equal((long)-(4 - index), (long) elem->j);
  });
}

static void test_carray_iterator_sort() {
  CArrayTestStruct array[5];
  for(int i=0; i<5; ++i) {
    array[i].i = i;
    array[i].j = -i;
  }

  sort(CArray_it(array, 5, sizeof(CArrayTestStruct)), ^(const void* obj1, const void* obj2) {
    const CArrayTestStruct* elem1 = (const CArrayTestStruct*) obj1;
    const CArrayTestStruct* elem2 = (const CArrayTestStruct*) obj2;

    if(elem1->j < elem2->j) {
      return -1;
    }

    if(elem1->j > elem2->j) {
      return 1;
    }

    return 0;
  });

  for_each_with_index(CArray_it(array, 5, sizeof(CArrayTestStruct)), ^(void* obj, size_t index) {
    CArrayTestStruct* elem = (CArrayTestStruct*) obj;
    assert_equal((long)(4 - index), (long) elem->i);
    assert_equal((long)-(4 - index), (long) elem->j);
  });
}


int main() {
  start_tests("basic iterators");

  test(test_int_iterator);
  test(test_file_iterator);
  test(test_char_iterator_reverse);
  test(test_char_iterator_sort);
  test(test_carray_iterator);
  test(test_carray_iterator_reverse);
  test(test_carray_iterator_sort);

  end_tests();
  return 0;
}
