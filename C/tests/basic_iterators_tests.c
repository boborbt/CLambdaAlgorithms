#include "basic_iterators.h"
#include "unit_testing.h"
#include "iterator_functions.h"

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

int main() {
  start_tests("basic iterators");

  test(test_int_iterator);
  test(test_file_iterator);

  end_tests();
  return 0;
}
