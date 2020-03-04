#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "exsorting_dataset.h"
#include "errors.h"
#include "ansi_colors.h"
#include "mem.h"

Record* new_record(Array* fields) {
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
    result->field1 = Mem_strdup(Array_at(fields,1));
    result->field3 = strtod(Array_at(fields, 3), NULL);
    if(errno!=0) {
        Error_raise(Error_new(ERROR_FILE_READING, "Cannot parse double: %s", Array_at(fields,3)));
    }

    return result;
}

int Dataset_compare_field1(const void* e1, const void* e2) {
  const Record* r1 = (const Record*) e1;
  const Record* r2 = (const Record*) e2;

  return Key_string_compare(r1->field1, r2->field1);
}

int Dataset_compare_field2(const void* e1, const void* e2) {
  const Record* r1 = (const Record*) e1;
  const Record* r2 = (const Record*) e2;

  return Key_int_compare(&r1->field2, &r2->field2);
}

int Dataset_compare_field3(const void* e1, const void* e2) {
  const Record* r1 = (const Record*) e1;
  const Record* r2 = (const Record*) e2;

  return Key_double_compare(&r1->field3, &r2->field3);
}


int Dataset_compare_field1_g(const void* e1, const void* e2) {
  const Record* r1 = *(Record* const*) e1;
  const Record* r2 = *(Record* const*) e2;

  return Key_string_compare(r1->field1, r2->field1);
}

int Dataset_compare_field2_g(const void* e1, const void* e2) {
  const Record* r1 = *(Record* const*) e1;
  const Record* r2 = *(Record* const*) e2;

  return Key_int_compare(&r1->field2, &r2->field2);
}

int Dataset_compare_field3_g(const void* e1, const void* e2) {
  const Record* r1 = *(Record* const*) e1;
  const Record* r2 = *(Record* const*) e2;

  return Key_double_compare(&r1->field3, &r2->field3);
}

void Dataset_print(Array* array, int num) {
  printf( BYEL "%10s\t%20s\t%10s\t%20s\n" reset, "ID", "field1", "field2", "field3");
  for(int i=0; i<num; ++i) {
    Record* rec = Array_at(array, (unsigned long) i);
    printf("%10d\t%20s\t%10d\t%20.5f\n", rec->id, rec->field1, rec->field2, rec->field3);
  }
}
