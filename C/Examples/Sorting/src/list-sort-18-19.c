#include "list.h"
#include "print_time.h"
#include "basic_iterators.h"
#include "iterator_functions.h"
#include "mem.h"
#include "macros.h"
#include <assert.h>
#include <stdlib.h>

typedef enum {
    SORT,
    MERGE
} Commands;

typedef struct {
    Commands command;
    const char* program_name;
    const char* file_name1;
    const char* file_name2;
} Options;

static void print_usage(const char **argv) {
  printf("Usage: \n");
  printf("  %s -s <filename>\n", argv[0]);
  printf("or\n");
  printf(" %s -m <list1> <list2>\n", argv[0]);
}

static Options parse_options(int argc, char const **argv) {
  if (argc < 3) {
      print_usage(argv);
      exit(1);
  }

  Options result;
  result.program_name = argv[0];

  if( !strcmp(argv[1], "-s" ) ) {
      result.command = SORT;
      result.file_name1 = argv[2];
      result.file_name2 = NULL;
      return result;
  }

  if( !strcmp(argv[1], "-m") ) {
      if(argc < 4) {
          printf("Too few parameters for -m option.\n");
          print_usage(argv);
          exit(2);
      }

      result.command = MERGE;
      result.file_name1 = argv[2];
      result.file_name2 = argv[3];
      return result;
  }

  printf("Unrecognized parameter. Expected -m or -s and got '%s'", argv[1]);
  exit(3);
}




static void list_split(List* src, List* lhs, List* rhs) {
    __block int index = 0;
    for_each(List_it(src), ^(void* elem) {
        if(index++ % 2 == 0) {
            List_append(lhs, elem);
        } else {
            List_append(rhs, elem);
        }
    });
}

static void list_merge(List *dest, List *lhs, List *rhs, int (^comp)(void *, void *)) {
    ListNode* lhs_it = List_head(lhs);
    ListNode* rhs_it = List_head(rhs);
    ListNode* dest_it = List_head(dest);

    while(lhs_it != NULL && rhs_it != NULL) {
        assert(dest_it != NULL);

        void* lhs_elem = ListNode_get(lhs, lhs_it);
        void* rhs_elem = ListNode_get(rhs, rhs_it);
        if(comp(lhs_elem, rhs_elem) < 0) {
            ListNode_set(dest, dest_it, lhs_elem);
            lhs_it = List_next(lhs, lhs_it);
        } else {
            ListNode_set(dest, dest_it, rhs_elem);
            rhs_it = List_next(rhs, rhs_it);
        }

        dest_it = List_next(dest, dest_it);
    }

    while(lhs_it!=NULL) {
        assert(dest_it != NULL);
        void *lhs_elem = ListNode_get(lhs, lhs_it);
        ListNode_set(dest, dest_it, lhs_elem);
        lhs_it = List_next(lhs, lhs_it);
        dest_it = List_next(dest, dest_it);
    }

    while (rhs_it != NULL) {
        assert(dest_it != NULL);
        void *rhs_elem = ListNode_get(rhs, rhs_it);
        ListNode_set(dest, dest_it, rhs_elem);
        rhs_it = List_next(rhs, rhs_it);
        dest_it = List_next(dest, dest_it);
    }
}

static void list_sort(List* list, int(^comp)(void*, void*)) {
    if(List_size(list) < 2) {
        return;
    }

    if(List_size(list) > 1000000) {
        printf("Sorting list of size: %ld\n", List_size(list));
    }

    List* lhs = List_new();
    List* rhs = List_new();

    list_split(list, lhs, rhs);
    list_sort(lhs, comp);
    list_sort(rhs, comp);
    list_merge(list, lhs, rhs, comp);

    List_free(lhs, NULL);
    List_free(rhs, NULL);
}

static List *load_dataset(PrintTime* pt, const char* filename)
{
    __block List *result = List_new();

    PrintTime_print(pt, "Dataset load", ^{
      printf("Dataset load...\n");
      FILE *infile = fopen(filename, "r");

      while (!feof(infile)) {
          long int *number = (long int *)Mem_alloc(sizeof(long int));
          if (fscanf(infile, "%ld", number) == 1) {
              List_append(result, number);
          }
          else {
              Mem_free(number);
          }
      }

      fclose(infile);
    });

    return result;
}

static void mem_free(void* ptr) {
    Mem_free(ptr);
}

static void do_sort(Options options) {
  PrintTime *pt = PrintTime_new(NULL);
  PrintTime_add_header(pt, "esercizio", "sort");
  PrintTime_add_header(pt, "invocation", options.program_name);

  List *list = load_dataset(pt, options.file_name1);

  PrintTime_print(pt, "Sorting...", ^{
    list_sort(list, ^(void *lhs, void *rhs) {
      long int lhs_val = *(long int *)lhs;
      long int rhs_val = *(long int *)rhs;

      if (lhs_val < rhs_val) {
        return -1;
      } else if (lhs_val == rhs_val) {
        return 0;
      } else {
        return 1;
      }
    });

    __block ListNode *it = List_head(list);
    for_each(Number_it(10), ^(UNUSED(void *index)) {
      long int elem = *(long int *)ListNode_get(list, it);
      printf("%ld\n", elem);
      it = List_next(list, it);
    });
  });

  PrintTime_free(pt);

  List_free(list, mem_free);
}

static void do_merge(Options options) {
  PrintTime *pt = PrintTime_new(NULL);
  PrintTime_add_header(pt, "esercizio", "merge");
  PrintTime_add_header(pt, "invocation", options.program_name);

  List *list1 = load_dataset(pt, options.file_name1);
  List *list2 = load_dataset(pt, options.file_name2);

  PrintTime_print(pt, "Merging...", ^{
    printf("Merging...\n");
    __block List *dest = List_new();
    size_t len1 = List_size(list1);
    size_t len2 = List_size(list2);
    for_each(Number_it(len1+len2), ^(UNUSED(void* number)) {
        List_append(dest, NULL);
    });

    list_merge(dest, list1, list2, ^(void *lhs, void *rhs) {
      long int lhs_val = *(long int *)lhs;
      long int rhs_val = *(long int *)rhs;

      if (lhs_val < rhs_val) {
        return -1;
      } else if (lhs_val == rhs_val) {
        return 0;
      } else {
        return 1;
      }
    });

    __block ListNode* it = List_head(dest);
    for_each(Number_it(10), ^(UNUSED(void* index)) {
        long int elem = *(long int*) ListNode_get(dest, it);
        printf("%ld\n", elem);
        it = List_next(dest, it);
    });

    List_free(dest, NULL);
  });

  PrintTime_free(pt);

  List_free(list1, mem_free);
  List_free(list2, mem_free);
}

int main(int argc, char const *argv[]) {
    Options options = parse_options(argc, argv);

    switch(options.command) {
        case SORT:
            do_sort(options);
            break;
        case MERGE:
            do_merge(options);
            break;
    }

    Mem_check_and_report();
    return 0;
}
