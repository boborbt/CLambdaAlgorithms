#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "ordered_array.h"

// --------------------------------------------------------------------------------
// Constants and Data structures
// --------------------------------------------------------------------------------

#define ARGUMENT_ERROR 1
#define FILE_ACCESS_ERROR 2
#define FILE_READ_ERROR 3

#define MAX_BUF 1024

typedef enum {
  SORT_STRINGS,
  SORT_INTS
} SortingOption;

typedef struct {
  const char* filename;
  SortingOption sorting_option;
} Options;

typedef struct {
  char* field1;
  int field2;
} Record;

// --------------------------------------------------------------------------------
// Comparison callbacks
// --------------------------------------------------------------------------------

static int compare_int(Record* obj1, Record* obj2) {
  if(obj1->field2 == obj2->field2) {
    return 0;
  }

  if(obj1->field2 < obj2->field2) {
    return -1;
  }

  return 1;
}

static int compare_str(Record* obj1, Record* obj2) {
  return strcmp(obj1->field1, obj2->field1);
}


// --------------------------------------------------------------------------------
// Option Handling
// --------------------------------------------------------------------------------

static void print_usage(char const* msg) {
  if(msg!=NULL) {
    printf("\nERROR: %s\n", msg);
  }

  printf("\nUsage:\n");
  printf("\tordered_array [-s] [-i] filename\n\n");
  printf("Reads the given file, sorts it, and print it on the console.\n\n");
}

static SortingOption parse_sorting_options(char const* option) {
  if(strlen(option) != 2 || option[0] != '-') {
    print_usage("First argument must specify the sorting option");
    exit(ARGUMENT_ERROR);
  }

  switch(option[1]) {
    case 's': return SORT_STRINGS;
    case 'i': return SORT_INTS;
    default:
      print_usage("Sorting option needs to be either 's' or 'i'");
      exit(ARGUMENT_ERROR);
  }
}

static char const* parse_filename(char const* filename) {
  if(access(filename, R_OK)!=0) {
    print_usage("File not found or not readable.");
    exit(FILE_ACCESS_ERROR);
  }

  return filename;
}

static Options parse_arguments(int argc, char const* argv[]) {
  Options options;

  if(argc < 3) {
    print_usage("Wrong number of parameters");
    exit(ARGUMENT_ERROR);
  }

  options.sorting_option = parse_sorting_options(argv[1]);
  options.filename = parse_filename(argv[2]);

  return options;
}

static CompareCallback sorting_function(Options options) {
  switch(options.sorting_option) {
    case SORT_STRINGS:
      return (CompareCallback) compare_str;
    case SORT_INTS:
      return (CompareCallback) compare_int;
  }
}

// --------------------------------------------------------------------------------
// Main implementation
// --------------------------------------------------------------------------------


static Record* parse_record(const char* string) {
  char str_buf[MAX_BUF];
  int  int_buf;


  int n_fields = sscanf(string, "%[^,],%d", str_buf, &int_buf);
  if(n_fields < 2) {
    printf("Error parsing line: %s\n", string);
    printf("Only %d fields successfully parsed", n_fields);
    exit(FILE_READ_ERROR);
  };

  Record* record = (Record*) malloc(sizeof(Record));

  record->field1 = strdup(str_buf);
  record->field2 = int_buf;

  return record;
}


static void read_into_array(Options options, OrderedArray* array) {
  FILE* file = fopen(options.filename, "r");
  char* buf = NULL;
  size_t buf_size = 0;

  while(getline(&buf, &buf_size, file) > 0) {
    OrderedArray_add(array, parse_record(buf));
  }

  fclose(file);
}

static void print_array(OrderedArray* array) {
  long size = OrderedArray_count(array);

  printf("\n%40s\t%10s\n\n", "[field1]", "[field2]");

  for(int i=0; i<size; ++i) {
    Record* record = (Record*) OrderedArray_at(array, i);
    printf("%40s\t%10d\n", record->field1, record->field2);
  }
}

static void free_array_contents(OrderedArray* array) {
  long size = OrderedArray_count(array);


  for(int i=0; i<size; ++i) {
    Record* record = OrderedArray_at(array, i);
    free(record->field1);
    free(record);
  }
}

static void read_and_print_file(Options options) {
  OrderedArray* array = OrderedArray_new(sorting_function(options));

  read_into_array(options, array);
  print_array(array);

  free_array_contents(array);
  OrderedArray_free(array);
}

// --------------------------------------------------------------------------------
// Main
// --------------------------------------------------------------------------------

int main(int argc, char const *argv[]) {
  
  Options options = parse_arguments(argc, argv);

  read_and_print_file(options);

  return 0;
}
