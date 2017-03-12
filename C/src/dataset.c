#include "dataset.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

#include "array.h"
#include "dictionary.h"
#include "errors.h"
#include "iterator_functions.h"
#include "mem.h"


struct _Record {
  int id;
  int field2;
  char* field1;
  double field3;
};

struct _Dataset {
  Array* records;
};


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

size_t Dataset_hash_field1(const void* e1) {
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

size_t Dataset_hash_field2(const void* e) {
  const Record* r = (const Record*) e;
  int value = r->field2;
  return (size_t)(value * (value+3));
}

size_t Dataset_hash_field3(const void* e) {
  const Record* r = (const Record*) e;
  double value = r->field3;
  return (size_t) (value * (value+3));
}



char* Record_get_field1(Record* record) {
  return record->field1;
}

int Record_get_field2(Record* record) {
  return record->field2;
}

double Record_get_field3(Record* record) {
  return record->field3;
}

static Record* parse_record(const char* str) {
  Record* record = (Record*) Mem_alloc(sizeof(Record));
  assert(record);

  char buf[2048];

  int num_fields = sscanf(str, "%d,%[^,],%d,%lf", &record->id, buf, &record->field2, &record->field3 );

  if(num_fields!=4) {
    Error_raise(Error_new(ERROR_FILE_READING, "Read only %d fields on line %s", num_fields, str));
  }

  record->field1 = Mem_strdup(buf);

  return record;
}

Dataset* Dataset_load(const char* filename) {
  Dataset* dataset = (Dataset*) Mem_alloc(sizeof(Dataset));
  assert(dataset!=NULL);

  dataset->records = Array_new(10000);
  assert(dataset->records != NULL);

  // field1 is usually small, much smaller than the alloced 2048 characters.
  char* buf = (char*) Mem_alloc(sizeof(char)*2048);
  assert(buf != NULL);

  size_t buf_len = 2048;
  size_t count = 0;

  FILE* file = fopen(filename, "r");
  if(!file) {
    Error_raise(Error_new(ERROR_FILE_READING, strerror(errno)));
  }
  while(!feof(file)) {
    if(count++ % 1000000 == 0) {
      printf(".");
      fflush(stdout);
    }

    if( getline((char**)&buf, &buf_len, file) == -1 ) {
      break;
    }
    Record* tmp = parse_record(buf);
    Array_add(dataset->records, tmp);
  }
  fclose(file);
  Mem_free(buf);

  printf("\n");

  if(Array_size(dataset->records) != 20000000) {
    printf("Warning reading datafile, only %ld records successfully read", Array_size(dataset->records));
  }

  return dataset;
}

Record** Dataset_get_records(Dataset* dataset) {
  return (Record**) Array_carray(dataset->records);
}

Array* Dataset_get_storage(Dataset* dataset) {
  return dataset->records;
}

size_t Dataset_size(Dataset* dataset) {
  return Array_size(dataset->records);
}

void Dataset_free(Dataset* dataset) {
  for_each(Array_it(dataset->records), ^(void* elem) {
    Record* record = elem;
    Mem_free(record->field1);
    Mem_free(record);
  });

  Array_free(dataset->records);
  Mem_free(dataset);
}

void Dataset_print_storage(Array* dataset, size_t num_records) {
  assert(num_records < Array_size(dataset));
  for(size_t i=0; i<num_records; ++i) {
    Record* rec = Array_at(dataset,i);
    printf("%10d %30s %10d %10.4f\n", rec->id, rec->field1, rec->field2, rec->field3);
  }
}

void Dataset_print(Dataset* dataset, size_t num_records) {
  assert(num_records < Dataset_size(dataset));
  for(size_t i=0; i<num_records; ++i) {
    Record* rec = Array_at(dataset->records,i);
    printf("%10d %30s %10d %10.4f\n", rec->id, rec->field1, rec->field2, rec->field3);
  }
}
