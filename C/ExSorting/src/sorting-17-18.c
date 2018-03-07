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
  const char* int_file;
  const char* sum_file;

  PrintTime* pt;

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

  result.int_file = argv[2];

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
        result.parse_error = "Option '-f' requires both a data file name and a sum file name";
        return result;
      }

      result.operation = OP_FIND_SUM;
      result.sum_file = argv[3];

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
  printf("   sorting-17-18 -s <file name>\n");
  printf("or sorting-17-18 -f <data file name> <sum file name>\n");
  printf("\n");
  printf("\nOptions:\n");
  printf(" -s: sorts the given file\n");
  printf(" -f: find pair of values in the given file such that their sum is equal\n");
  printf("     to the sums provided in the 'sum file'\n");
}

static Array* load_dataset(Options options) {
  __block Array* result = Array_new(5000001);

  PrintTime_print(options.pt, "Dataset load", ^{
    printf("Dataset load...\n");
    FILE* infile = fopen(options.int_file, "r");

    while(!feof(infile)) {
      long int* number = (long int*) Mem_alloc(sizeof(long int));
      if(fscanf(infile, "%ld", number) == 1) {;
        Array_add(result, number);
      } else {
        Mem_free(number);
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
    Array_sort(array, ^(const void* lhs, const void* rhs) {
      return Key_long_compare(lhs, rhs);
    });
  });

  for(unsigned int i=0; i<10; ++i) {
    printf("%d: %ld\n", i, *(long int*)Array_at(array, i));
  }

  PrintTime_print(options.pt, "Freeing memory...", ^{
    printf("Freeing memory...\n");
    free_dataset_contents(array);
    Array_free(array);
  });
}

static int find_sum(Array* array, long int sum, long int* first, long int* second) {
  size_t start = 0;
  size_t end = Array_size(array) - 1;
  int found = 0;

  while( start < end && !found ) {
    long int low = *(long int*) Array_at(array, start);
    long int high = *(long int*) Array_at(array, end);
    if(low + high == sum) {
      found = 1;
    } else if(low + high < sum) {
      start+=1;
    } else {
      end-=1;
    }
  }

  if(found) {
    *first = *(long int*) Array_at(array, start);
    *second = *(long int*) Array_at(array, end);
  }

  return found;
}

static Array* load_sums(Options options) {
  FILE* infile = fopen(options.sum_file, "r");
  Array* sums = Array_new(100);

  while(!feof(infile)) {
    long int* sum = Mem_alloc(sizeof(long int));

    if(fscanf(infile, "%ld", sum) == 1) {
      Array_add(sums, sum);
    } else {
      Mem_free(sum);
    }
  }

  return sums;
}


static void find_sums(Options options) {

  Array* array = load_dataset(options);
  Array* sums = load_sums(options);

  PrintTime_print(options.pt, "Sorting...", ^{
    printf("Sorting...\n");
    Array_sort(array, ^(const void* lhs, const void* rhs) {
      return Key_long_compare(lhs, rhs);
    });
  });


  PrintTime_print(options.pt, "Searching...", ^{
    printf("Searching sums...\n");

    for_each(Array_it(sums), ^(void* sum_obj) {
      long int sum = *(long int*) sum_obj;
      long int first, second;
      int found = find_sum(array, sum, &first, &second);

      if(found) {
        printf("Found values %ld, %ld whose sum is exactly %ld\n",
            first, second, sum);
      } else {
        printf("Could not find any pair of integers whose sum is %ld\n", sum);
      }
    });
  });

  PrintTime_print(options.pt, "Freeing memory...", ^{
    free_dataset_contents(array);
    free_dataset_contents(sums);
    Array_free(sums);
    Array_free(array);
  });
}


int main(int argc, char const *argv[]) {
  Options options = parse_args(argc, argv);

  if(!options.check_ok) {
    printf("Error while parsing options: %s\n", options.parse_error);
    print_usage();
    exit(1);
  }

  switch(options.operation) {
    case OP_SORT:
      sort_file(options);
      break;
    case OP_FIND_SUM:
      find_sums(options);
      break;
  }

  free_options(options);
  Mem_check_and_report();

  return 0;
}
