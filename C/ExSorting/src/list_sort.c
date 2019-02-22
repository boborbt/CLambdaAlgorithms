#include "list.h"
#include "print_time.h"
#include "basic_iterators.h"
#include "iterator_functions.h"
#include "mem.h"
#include <assert.h>
#include <stdlib.h>

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

int main(int argc, char const *argv[]) {
    if(argc < 2) {
        printf("Usage: \n");
        printf("  %s <filename>", argv[0]);
        exit(1);
    }

    PrintTime* pt = PrintTime_new(NULL);
    PrintTime_add_header(pt, "esercizio", "1");
    PrintTime_add_header(pt, "invocation", argv[0]);

    List *list = load_dataset(pt, argv[1]);


    PrintTime_print(pt, "Sorting...", ^{
        list_sort(list, ^(void* lhs, void* rhs) {
            long int lhs_val = *(long int*) lhs;
            long int rhs_val = *(long int*) rhs;

            if(lhs_val < rhs_val) {
                return -1;
            } else if(lhs_val == rhs_val) {
                return 0;
            } else {
                return 1;
            }
        });
    });

    // PrintTime_print(pt, "Sorting...", ^{
    //     sort(List_it(list), ^(const void *lhs, const void *rhs){
    //         long int lhs_val = *(const long int*) lhs;
    //         long int rhs_val = *(const long int*) rhs;

    //         if(lhs_val < rhs_val) {
    //             return -1;
    //         } else if(lhs_val == rhs_val) {
    //             return 0;
    //         } else {
    //             return 1;
    //         }
    //     });
    // });

        PrintTime_free(pt);

        List_free(list, mem_free);

        Mem_check_and_report();
        return 0;
}
