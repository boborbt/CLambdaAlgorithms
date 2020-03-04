#include "dataset_.h"
#include "iterator_functions.h"
#include "basic_iterators.h"
#include "string_utils.h"
#include "ansi_colors.h"

Array* Dataset__load(const char* filename, unsigned long max_lines, void* (^load_fun)(Array* fields)) {
    Array* result = Array_new(1000);
    __block unsigned long count = 0;

    printf("\n");
    find_first( TextFile_it(filename, '\n'), ^(void* fields) {
        if(count++ >= max_lines) {
            return 1;
        }

        if(count % 10000 == 0) {
            printf(GRN "Read " BWHT "%lu" GRN "K records...\r" reset, count/1000);
        }

        char* str = (char*) fields;

        Array_add(result, load_fun(String_split(str, ',')));
        return 0;
    });
    printf("\n");

    return result;
}
