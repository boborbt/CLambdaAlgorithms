#pragma once

#include <stdlib.h>

// Opaque data structures
typedef struct _Record Record;
typedef struct _Dataset Dataset;

// Comparison functions based on the three fields defined in Record
// g versions assume the parameters point to pointers to the objects
// to be compared. non g version assume the paramenters point to the actual
// objects
int Dataset_compare_field1(const void* e1, const void* e2);
int Dataset_compare_field2(const void* e1, const void* e2);
int Dataset_compare_field3(const void* e1, const void* e2);

int Dataset_compare_field1_g(const void* e1, const void* e2);
int Dataset_compare_field2_g(const void* e1, const void* e2);
int Dataset_compare_field3_g(const void* e1, const void* e2);


// Hash functions based on the three fields defined in Record
size_t Dataset_hash_field1(const void* e);
size_t Dataset_hash_field2(const void* e);
size_t Dataset_hash_field3(const void* e);

// Field accessors
char* Record_get_field1(Record* record);
int Record_get_field2(Record* record);
double Record_get_field3(Record* record);

// Loads a dataset from file allocing all memory necessary
Dataset* Dataset_load(const char* filename);
// Disposes a dataset freeing all alloced memory
void Dataset_free(Dataset* dataset);

// Print on the stdout the first num_records of the given dataset
void Dataset_print(Dataset* dataset, size_t num_records);

// Returns the array of records stored in dataset
Record** Dataset_get_records(Dataset* dataset);

// Returns the size of the dataset (i.e., the number of records stored in
// this dataset)
size_t Dataset_size(Dataset* size);
