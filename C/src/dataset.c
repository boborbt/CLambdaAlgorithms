#include "dataset.h"
#include "iterator_functions.h"
#include "basic_iterators.h"
#include "string_utils.h"
#include "ansi_colors.h"
#include "mem.h"
#include "iterator_functions.h"

static DatasetOpts defaults = { (unsigned long) -1, 100, 1024 };

Array* Dataset__load(const char* filename, DatasetOpts* options, void* (^load_fun)(Array* fields)) {
    Array* result = Array_new(1000);
    __block unsigned long count = 0;

    if(options == NULL) {
        options = &defaults;
    }

    Array* fields_array = Array_new(options->num_fields);
    for(size_t i=0; i<options->num_fields; ++i) {
        Array_add(fields_array, Mem_alloc(sizeof(char)*options->max_field_len));
    }

    printf("\n");
    find_first( TextFile_it(filename, '\n'), ^(void* fields) {
        if(count++ >= options->max_lines) {
            return 1;
        }

        if(count % 10000 == 0) {
            printf(GRN "Read " BWHT "%lu" GRN "K records...\r" reset, count/1000);
        }

        char* str = (char*) fields;
        String_fast_split(str, ',', fields_array, 1024);
        Array_add(result, load_fun(fields_array));

        return 0;
    });
    printf("\n");

    for_each(Array_it(fields_array), ^(void* obj) {
        Mem_free(obj);
    });
    Array_free(fields_array);

    return result;
}
