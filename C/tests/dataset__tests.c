#include <inttypes.h>
#include <errno.h>

#include "unit_testing.h"
#include "dataset_.h"
#include "mem.h"
#include "errors.h"

typedef struct {
  int id;
  int field2;
  char* field1;
  double field3;
} Record;

static Record* new_record(Array* fields) {
    if(Array_size(fields) != 4) {
        Error_raise(Error_new(ERROR_FILE_READING, "Expected 4 fields, but %lu found", Array_size(fields)));
    }

    Record* result = (Record*) Mem_alloc(sizeof(Record));
    errno = 0;
    result->id = (int) strtol(Array_at(fields, 0), NULL, 10);
    if(errno!=0) {
        Error_raise(Error_new(ERROR_FILE_READING, "Cannot parse integer: %s", Array_at(fields,0)));
    }
    result->field2 = (int) strtol(Array_at(fields, 2), NULL, 10);
    if(errno!=0) {
        Error_raise(Error_new(ERROR_FILE_READING, "Cannot parse integer: %s", Array_at(fields,1)));
    }
    result->field1 = strdup(Array_at(fields,1));
    result->field3 = strtod(Array_at(fields, 3), NULL);
    if(errno!=0) {
        Error_raise(Error_new(ERROR_FILE_READING, "Cannot parse double: %s", Array_at(fields,3)));
    }

    return result;
}

static void test_dataset__load() {
    DatasetOpts options = { 10l, 4, 1024 };
    Array* ds = Dataset__load("records.csv",  &options, ^(Array* fields) { return (void*) new_record(fields); });
    assert_equal( 10l, Array_size(ds) );
    Record* rec1 = Array_at(ds, 0);
    assert_equal((long) rec1->id, 0l);
    assert_string_equal(rec1->field1, "noto");
    assert_equal((long) rec1->field2, 233460l);
    assert_double_equal(rec1->field3, 32209.073312, 0.0001);
}


int main() {
  start_tests("dataset_");

  test(test_dataset__load);

  end_tests();

  return 0;
}
