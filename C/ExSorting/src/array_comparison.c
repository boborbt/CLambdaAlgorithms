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

struct _Record {
  int id;
  int field2;
  char* field1;
  double field3;
};

typedef struct _Record Record;

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

static void array_exp(const char* filename) {
  PrintTime* pt = PrintTime_new(NULL);
  PrintTime_add_header(pt, "array_comp", "array");

  Array* array = Array_new(1000);

  PrintTime_print(pt, "array load", ^{
    printf("Array -- loading dataset \n");
    Dataset_load(filename, array, (void (*)(void*,Record)) Array_custom_add);
  });

  PrintTime_print(pt, "array it", ^{
    printf("Array: summing all field3 elems\n");
    __block double sum = 0.0;

    for(int i=0; i<20; ++i) {
      for_each(Array_it(array), ^(void* elem) {
        sum += ((Record*) elem)->field3;
      });
    }

    printf("Array sum: %f\n", sum);
  });

  PrintTime_print(pt, "array sort", ^{
  printf("Array -- sort\n");

  sort(Array_it(array), ^(const void* lhs_obj, const void* rhs_obj) {
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

  PrintTime_print(pt, "array it", ^{
    printf("ArrayAlt: summing all field3 elems\n");
    __block double sum = 0.0;

    for(int i=0; i<20; ++i) {
      for_each(ArrayAlt_it(array_alt), ^(void* elem) {
        sum += ((Record*) elem)->field3;
      });
    }

    printf("ArrayAlt sum: %f\n", sum);
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


int main(int argc, char const *argv[]) {
  if(argc < 2) {
    printf("Arguments error\n");
    exit(1);
  }

  array_alt_exp(argv[1]);
  Mem_check_and_report();

  array_exp(argv[1]);
  Mem_check_and_report();

  return 0;
}
