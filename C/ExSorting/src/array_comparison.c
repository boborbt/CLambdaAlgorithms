#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "array.h"
#include "array_alt.h"
#include "errors.h"
#include "mem.h"
#include "print_time.h"
#include "iterator_functions.h"
#include "basic_iterators.h"

#define DATASET_SIZE 20000000

struct _Record {
  int id;
  int field2;
  char* field1;
  double field3;
};

typedef struct _Record Record;

typedef struct  {
  size_t count;
  Record array[DATASET_SIZE];
} RecordArray;

static Record parse_record(const char* str) {
  Record record;
  char buf[2048];

  int num_fields = sscanf(str, "%d,%[^,],%d,%lf", &record.id, buf, &record.field2, &record.field3 );

  if(num_fields!=4) {
    Error_raise(Error_new(ERROR_FILE_READING, "Read only %d fields on line %s", num_fields, str));
  }

  record.field1 = Mem_strdup(buf);

  return record;
}

static void Dataset_load(const char* filename, void* container, void (*container_add)(void*, Record)) {
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
    Record tmp = parse_record(buf);
    container_add(container, tmp);
  }
  fclose(file);
  Mem_free(buf);

  printf("\n");
}


static void Array_custom_add(Array* array, Record record) {
  Record* new_record = (Record*) Mem_alloc(sizeof(Record));
  memcpy(new_record, &record, sizeof(Record));

  Array_add(array, new_record);
}

static void Array_alt_custom_add(ArrayAlt* array, Record record) {
  ArrayAlt_add(array, &record);
}

static void carray_custom_add(RecordArray* carray, Record record) {
  if(carray->count >= DATASET_SIZE) {
    printf("Error: Trying to insert too many records into carray...\n");
    exit(1);
  }
  memcpy(&carray->array[carray->count++], &record, sizeof(record));
}

static void array_exp(const char* filename) {
  PrintTime* pt = PrintTime_new(NULL);
  PrintTime_add_header(pt, "array_comp", "array");

  Array* array = Array_new(1000);

  PrintTime_print(pt, "array load", ^{
    printf("Array -- loading dataset \n");
    Dataset_load(filename, array, (void (*)(void*,Record)) Array_custom_add);
  });

  PrintTime_print(pt, "array sort", ^{
    printf("Array -- sort\n");

    Array_sort(array, ^(const void* lhs_obj, const void* rhs_obj) {
      const Record* lhs = (const Record*) lhs_obj;
      const Record* rhs = (const Record*) rhs_obj;

      if(lhs->field2 < rhs->field2) {
        return -1;
      }
      if(lhs->field2 > rhs->field2) {
        return 1;
      }
      return 0;
    });
  });

  PrintTime_print(pt, "checking array order", ^{
    printf("Array: Checking array order\n");
    __block Record* last_record = NULL;

    void* found = find_first(Array_it(array), ^int (void* obj) {
      Record* record = (Record*) obj;
      if(last_record!=NULL && record->field2 < last_record->field2) {
        return 1;
      }
      last_record = obj;
      return 0;
    });

    if(found) {
      printf("Order check failed\n");
    } else {
      printf("Order check completed with success.\n");
    }
  });

  PrintTime_print(pt, "array dealloc", ^{
    printf("Array -- freeing dataset \n");

    for_each(Array_it(array), ^(void* elem) {
      Record* rec = (Record*) elem;
      Mem_free(rec->field1);
      Mem_free(elem);
    });

    Array_free(array);
  });

  PrintTime_free(pt);
}

static void array_alt_exp(const char* filename) {
  PrintTime* pt = PrintTime_new(NULL);
  PrintTime_add_header(pt, "array_alt_comp", "array");

  ArrayAlt* array_alt = ArrayAlt_new(1000, sizeof(Record));

  PrintTime_print(pt, "array alt load", ^{
    printf("ArrayAlt -- loading dataset \n");
    Dataset_load(filename, array_alt, (void (*)(void*,Record)) Array_alt_custom_add);
  });


  PrintTime_print(pt, "array alt sort", ^{
    printf("ArrayAlt -- sort\n");

    void* carray = ArrayAlt_carray(array_alt);
    size_t count = ArrayAlt_size(array_alt);
    size_t width = sizeof(Record);
    sort(CArray_it(carray, count, width), ^(const void* lhs_obj, const void* rhs_obj) {
      const Record* lhs = (const Record*) lhs_obj;
      const Record* rhs = (const Record*) rhs_obj;

      if(lhs->field2 < rhs->field2) {
        return -1;
      }
      if(lhs->field2 > rhs->field2) {
        return 1;
      }
      return 0;
    });
  });

  PrintTime_print(pt, "checking array order", ^{
    printf("ArrayAlt: Checking array order\n");
    __block Record* last_record = NULL;

    void* found = find_first(ArrayAlt_it(array_alt), ^int (void* obj) {
      Record* record = (Record*) obj;
      if(last_record!=NULL && record->field2 < last_record->field2) {
        return 1;
      }
      last_record = obj;
      return 0;
    });

    if(found) {
      printf("Order check failed\n");
    } else {
      printf("Order check completed with success.\n");
    }
  });


  PrintTime_print(pt, "array alt dealloc", ^{
    printf("ArrayAlt -- freeing dataset \n");

    for_each(ArrayAlt_it(array_alt), ^(void* elem){
      Record* rec = (Record*) elem;
      Mem_free(rec->field1);
    });
    ArrayAlt_free(array_alt);
  });

  PrintTime_free(pt);
}

static void carray_exp(const char* filename) {
  PrintTime* pt = PrintTime_new(NULL);
  PrintTime_add_header(pt, "carray_comp", "array");

  RecordArray* carray = (RecordArray*) Mem_alloc(sizeof(RecordArray));
  carray->count = 0;

  PrintTime_print(pt, "array load", ^{
    printf("CArray -- loading dataset \n");
    Dataset_load(filename, carray, (void (*)(void*,Record)) carray_custom_add);
    printf("Loaded %zu objects\n", carray->count);
  });

  PrintTime_print(pt, "array sort", ^{
    printf("CArray -- sort\n");

    sort(CArray_it(carray->array, carray->count, sizeof(Record)), ^(const void* lhs_obj, const void* rhs_obj) {
      const Record* lhs = (const Record*) lhs_obj;
      const Record* rhs = (const Record*) rhs_obj;

      if(lhs->field2 < rhs->field2) {
        return -1;
      }
      if(lhs->field2 > rhs->field2) {
        return 1;
      }
      return 0;
    });
  });

  PrintTime_print(pt, "checking array order", ^{
    printf("CArray: Checking array order\n");
    __block Record* last_record = NULL;

    void* found = find_first(CArray_it(carray->array, carray->count, sizeof(Record)), ^int (void* obj) {
      Record* record = (Record*) obj;
      if(last_record!=NULL && record->field2 < last_record->field2) {
        return 1;
      }
      last_record = obj;
      return 0;
    });

    if(found) {
      printf("Order check failed\n");
    } else {
      printf("Order check completed with success.\n");
    }
  });

  PrintTime_print(pt, "array dealloc", ^{
    printf("CArray -- freeing dataset \n");

    Mem_free(carray);
  });

  PrintTime_free(pt);
}


int main(int argc, char const *argv[]) {
  if(argc < 2) {
    printf("Arguments error\n");
    exit(1);
  }

  carray_exp(argv[1]);
  Mem_check_and_report();

  array_alt_exp(argv[1]);
  Mem_check_and_report();

  array_exp(argv[1]);
  Mem_check_and_report();



  return 0;
}
