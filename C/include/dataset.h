#pragma once

#include "array.h"

typedef struct {
    // defaults (unsigned long) -1, i.e, to load the whole dataset, if you want to 
    // read only a part of the dataset set this parameter to
    // the number of records you want to read.
    unsigned long max_lines;

    // defaults to 100. This means that the load_fun will be provided
    // with 100 fields regardless of the number of fields actually read
    // from the current line. Set this field to a proper number if you
    // need either:
    //    - to read a larger number of fields
    //    - to read a smaller number of fields and you require that laod_fun
    //      is provided with exactly the number of fields you need
    unsigned long num_fields;

    // defaults to 1024. Set it to a different number if you want to alloc
    //    a larger or a smaller number of character per field.
    unsigned long max_field_len;
    
} DatasetOpts;

// Reads the dataset contained in the file named <filename>.
// If the options pointer is null defaults are chosen as specified in the
// description of the DatasetOpts structure.
Array* Dataset__load(const char* filename, DatasetOpts* options, void* (^load_fun)(Array* fields));
