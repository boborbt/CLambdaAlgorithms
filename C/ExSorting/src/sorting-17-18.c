#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "array.h"
#include "iterator_functions.h"
#include "keys.h"
#include "mem.h"
#include "print_time.h"

typedef enum {
  OP_SORT,
  OP_FIND_SUM
} Operation;

typedef struct {
  const char* parse_error;
  const char* input_file;
  PrintTime* pt;
  long int desired_sum;

  Operation operation;
  int check_ok;
} Options;


static PrintTime* init_print_time(const char* argv[]) {
  PrintTime* pt = PrintTime_new(NULL);

  PrintTime_add_header(pt, "esercizio", "1");
  PrintTime_add_header(pt, "invocation", argv[0]);

  return pt;
}

static Options parse_args(int argc, const char* argv[]) {
  Options result;
  result.check_ok = 0;

  result.pt = init_print_time(argv);

  if(argc < 3) {
    result.parse_error = "Wrong number of parameters";
    return result;
  }

  result.input_file = argv[2];

  const char* op_str = argv[1];

  if( strcmp(op_str, "-s") != 0 && strcmp(op_str, "-f") != 0 ) {
    result.parse_error = "First option must be either '-s' or '-f'";
    return result;
  }

  switch(op_str[1]) {
    case 's':
      result.operation = OP_SORT;
      break;
    case 'f':
      if(argc < 4) {
        result.parse_error = "Option '-f' requires both a filename and a sum value";
        return result;
      }

      errno = 0;

      result.operation = OP_FIND_SUM;
      result.desired_sum = strtol(argv[3], NULL, 0);

      if(errno != 0) {
        result.parse_error = "Error parsing the given sum value";
        return result;
      }

      break;
  }

  result.check_ok = 1;
  return result;
}


static void free_options(Options options) {
  PrintTime_free(options.pt);
}


static void print_usage() {
  printf("Usage:\n");
  printf("   sorting-17-18 -s <filename>\n");
  printf("or sorting-17-18 -f <filename> <sumvalue>\n");
  printf("\n");
  printf("\nOptions:\n");
  printf(" -s: sorts the given file\n");
  printf(" -f: find two values in the given file such that their sum is equal\n");
  printf("     to the specified value\n");
}

static Array* load_dataset(Options options) {
  __block Array* result = Array_new(5000001);

  PrintTime_print(options.pt, "Dataset load", ^{
    printf("Dataset load...\n");
    FILE* infile = fopen(options.input_file, "r");

    while(!feof(infile)) {
      long int* number = (long int*) Mem_alloc(sizeof(long int));
      fscanf(infile, "%ld", number);

      if(feof(infile)) {
        Mem_free(number);
      } else {
        Array_add(result, number);
      }
    }

    fclose(infile);
  });

  return result;
}

static void free_dataset_contents(Array* array) {
  for_each(Array_it(array), ^(void* elem) {
    Mem_free(elem);
  });
}

static void sort_file(Options options) {
  Array* array = load_dataset(options);

  PrintTime_print(options.pt, "Sorting...", ^{
    printf("Sorting\n");
    Array_sort(array, Key_long_compare);
  });

  for(unsigned int i=0; i<10; ++i) {
    printf("%d: %ld\n", i, *(long int*)Array_at(array, i));
  }

  free_dataset_contents(array);
  Array_free(array);
}


static void find_sum(Options options) {

  Array* array = load_dataset(options);


  PrintTime_print(options.pt, "Sorting...", ^{
    printf("Sorting...\n");
    Array_sort(array, Key_long_compare);
  });

  PrintTime_print(options.pt, "Searching...", ^{

    printf("Searching sum...\n");
    size_t start = 0;
    size_t end = Array_size(array) - 1;
    int found = 0;

    while( start < end && !found ) {
      long int low = *(long int*) Array_at(array, start);
      long int high = *(long int*) Array_at(array, end);
      if(low + high == options.desired_sum) {
        found = 1;
      } else if(low + high < options.desired_sum) {
        start+=1;
      } else {
        end-=1;
      }
    }

    if(found) {
      printf("Found values %ld, %ld whose sum is exactly %ld\n",
          *(long int*)Array_at(array, start),
          *(long int*)Array_at(array, end),
          options.desired_sum);
    } else {
      printf("Could not find any pair of integers whose sum is %ld\n", options.desired_sum);
    }
  });

  free_dataset_contents(array);
  Array_free(array);
}


int main(int argc, char const *argv[]) {
  Options options = parse_args(argc, argv);

  if(!options.check_ok) {
    print_usage();
    exit(1);
  }

  switch(options.operation) {
    case OP_SORT:
      sort_file(options);
      break;
    case OP_FIND_SUM:
      find_sum(options);
      break;
  }

  free_options(options);
  Mem_check_and_report();

  return 0;
}
