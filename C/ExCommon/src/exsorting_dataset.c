#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "exsorting_dataset.h"
#include "errors.h"
#include "ansi_colors.h"
#include "mem.h"
#include "dataset_.h"
#include "iterator_functions.h"

Record* new_record(Array* fields) {
    if(Array_size(fields) < 4) {
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

int ExSortingDataset_compare_field1(const void* e1, const void* e2) {
  const Record* r1 = (const Record*) e1;
  const Record* r2 = (const Record*) e2;

  return Key_string_compare(r1->field1, r2->field1);
}

int ExSortingDataset_compare_field2(const void* e1, const void* e2) {
  const Record* r1 = (const Record*) e1;
  const Record* r2 = (const Record*) e2;

  return Key_int_compare(&r1->field2, &r2->field2);
}

int ExSortingDataset_compare_field3(const void* e1, const void* e2) {
  const Record* r1 = (const Record*) e1;
  const Record* r2 = (const Record*) e2;

  return Key_double_compare(&r1->field3, &r2->field3);
}


int ExSortingDataset_compare_field1_g(const void* e1, const void* e2) {
  const Record* r1 = *(Record* const*) e1;
  const Record* r2 = *(Record* const*) e2;

  return Key_string_compare(r1->field1, r2->field1);
}

int ExSortingDataset_compare_field2_g(const void* e1, const void* e2) {
  const Record* r1 = *(Record* const*) e1;
  const Record* r2 = *(Record* const*) e2;

  return Key_int_compare(&r1->field2, &r2->field2);
}

int ExSortingDataset_compare_field3_g(const void* e1, const void* e2) {
  const Record* r1 = *(Record* const*) e1;
  const Record* r2 = *(Record* const*) e2;

  return Key_double_compare(&r1->field3, &r2->field3);
}

void ExSortingDataset_print(Array* array, int num) {
  printf( BYEL "%10s\t%20s\t%10s\t%20s\n" reset, "ID", "field1", "field2", "field3");
  for(int i=0; i<num; ++i) {
    Record* rec = Array_at(array, (unsigned long) i);
    printf("%10d\t%20s\t%10d\t%20.5f\n", rec->id, rec->field1, rec->field2, rec->field3);
  }
}

size_t ExSortingDataset_hash_field1(const void* e1) {
  const Record* r1 = (const Record*) e1;
  char* str = r1->field1;

  size_t h = 0;
  size_t len = strlen(str);
  for(size_t i=0; i<len; ++i) {
    size_t highorder = h & 0xf8000000;
    h = h << 5;                    // shift h left by 5 bits
    h = h ^ (highorder >> 27);     // move the highorder 5 bits to the low-order
    h = h ^ (size_t)str[i];                // XOR h and ki
  }
  return h;
}

size_t ExSortingDataset_hash_field2(const void* e) {
  const Record* r = (const Record*) e;
  int value = r->field2;
  return (size_t)(value * (value+3));
}

size_t ExSortingDataset_hash_field3(const void* e) {
  const Record* r = (const Record*) e;
  double value = r->field3;
  return (size_t) (value * (value+3));
}



Array* ExSortingDataset_load(const char* filename) {
  return Dataset__load(filename, NULL, ^(Array* fields) { return (void*) new_record(fields); });
}

void ExSortingDataset_free(Array* dataset) {
    for_each(Array_it(dataset), ^(void* obj) {
      Record* rec = (Record*) obj;
      Mem_free(rec->field1);
      Mem_free(rec);
    });

    Array_free(dataset);
}
