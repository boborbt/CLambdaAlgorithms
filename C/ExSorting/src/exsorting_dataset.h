#pragma once

#include "array.h"

typedef struct {
  int id;
  int field2;
  char* field1;
  double field3;
} Record;

Record* new_record(Array* fields);

int Dataset_compare_field1(const void* e1, const void* e2);
int Dataset_compare_field2(const void* e1, const void* e2);
int Dataset_compare_field3(const void* e1, const void* e2);
int Dataset_compare_field1_g(const void* e1, const void* e2);
int Dataset_compare_field2_g(const void* e1, const void* e2);
int Dataset_compare_field3_g(const void* e1, const void* e2);

void Dataset_print(Array* array, int num);
