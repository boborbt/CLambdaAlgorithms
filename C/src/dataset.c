#include "dataset.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "dictionary.h"

struct _Record {
  int id;
  int field2;
  char* field1;
  double field3;
};

struct _Dataset {
  size_t size;
  Record** records;
};


int Dataset_compare_field1(const void* e1, const void* e2) {
  const Record* r1 = (const Record*) e1;
  const Record* r2 = (const Record*) e2;

  return KeyInfo_string_compare(r1->field1, r2->field1);
}

int Dataset_compare_field2(const void* e1, const void* e2) {
  const Record* r1 = (const Record*) e1;
  const Record* r2 = (const Record*) e2;

  return KeyInfo_int_compare(&r1->field2, &r2->field2);
}

int Dataset_compare_field3(const void* e1, const void* e2) {
  const Record* r1 = (const Record*) e1;
  const Record* r2 = (const Record*) e2;

  return KeyInfo_double_compare(&r1->field3, &r2->field3);
}


int Dataset_compare_field1_g(const void* e1, const void* e2) {
  const Record* r1 = *(Record* const*) e1;
  const Record* r2 = *(Record* const*) e2;

  return KeyInfo_string_compare(r1->field1, r2->field1);
}

int Dataset_compare_field2_g(const void* e1, const void* e2) {
  const Record* r1 = *(Record* const*) e1;
  const Record* r2 = *(Record* const*) e2;

  return KeyInfo_int_compare(&r1->field2, &r2->field2);
}

int Dataset_compare_field3_g(const void* e1, const void* e2) {
  const Record* r1 = *(Record* const*) e1;
  const Record* r2 = *(Record* const*) e2;

  return KeyInfo_double_compare(&r1->field3, &r2->field3);
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
  Record* record = (Record*) malloc(sizeof(Record));
  assert(record);

  char buf[2048];

  int num_fields = sscanf(str, "%d,%[^,],%d,%lf", &record->id, buf, &record->field2, &record->field3 );

  if(num_fields!=4) {
    printf("Read only %d fields on line %s", num_fields, str );
    exit(1);
  }

  record->field1 = strdup(buf);

  return record;
}

Dataset* Dataset_load(const char* filename) {
  Dataset* dataset = (Dataset*) malloc(sizeof(Dataset));
  assert(dataset!=NULL);

  // we are cutting some corners here. Assuming the file contains exactly
  // 20_000_000 records.
  dataset->records = (Record**) malloc(sizeof(Record*) * 20000000);
  assert(dataset->records != NULL);

  // field1 is usually small, much smaller than the alloced 2048 characters.
  dataset->size = 0;
  char* buf = (char*) malloc(sizeof(char) * 2048 );
  assert(buf != NULL);

  size_t buf_len = 2048;

  FILE* file = fopen(filename, "r");
  if(!file) {
    perror(NULL);
    exit(1);
  }
  while(!feof(file)) {
    if( getline(&buf, &buf_len, file) == -1 ) {
      break;
    }

    if(dataset->size >= 20000000) {
      printf("Trying to read after alloced capacity, currently read %ld lines\nnext line:%s", dataset->size, buf);
      exit(1);
    }

    dataset->records[dataset->size++] = parse_record(buf);
  }
  fclose(file);
  free(buf);

  if(dataset->size != 20000000) {
    printf("Warning reading datafile, only %ld records successfully read", dataset->size);
  }

  return dataset;
}

Record** Dataset_get_records(Dataset* dataset) {
  return dataset->records;
}

size_t Dataset_size(Dataset* dataset) {
  return dataset->size;
}

void Dataset_free(Dataset* dataset) {
  for(size_t i=0; i<dataset->size; ++i) {
    free(dataset->records[i]->field1);
    free(dataset->records[i]);
  }

  free(dataset->records);
  free(dataset);
}

void Dataset_print(Dataset* dataset, size_t num_records) {
  assert(num_records < dataset->size);
  for(size_t i=0; i<num_records; ++i) {
    Record* rec = dataset->records[i];
    printf("%10d %30s %10d %10.4f\n", rec->id, rec->field1, rec->field2, rec->field3);
  }
}
