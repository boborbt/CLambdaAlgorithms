#ifndef __DATASET_H_KJADFLM83NX83__
#define __DATASET_H_KJADFLM83NX83__

// Opaque data structures
typedef struct _Record Record;
typedef struct _Dataset Dataset;

// Comparison functions based on the three fields defined in Record
int Dataset_compare_field1(const void* e1, const void* e2);
int Dataset_compare_field2(const void* e1, const void* e2);
int Dataset_compare_field3(const void* e1, const void* e2);

// Loads a dataset from file allocing all memory necessary
Dataset* Dataset_load(const char* filename);
// Disposes a dataset freeing all alloced memory
void Dataset_free(Dataset* dataset);
// Print on the stdout the first num_records of the given dataset
void Dataset_print(Dataset* dataset, int num_records);

// Returns the array of records stoed in dataset
Record** Dataset_get_records(Dataset* dataset);

// Returns the size of the dataset (i.e., the number of records stored in
// this dataset)
int Dataset_get_size(Dataset* size);

#endif
