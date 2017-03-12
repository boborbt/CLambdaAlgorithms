#include <stdlib.h>
#include "double_container.h"
#include "mem.h"


DoubleContainer* DoubleContainer_new(double value) {
  double* result = (double*) Mem_alloc(sizeof(double));
  *result = value;
  return result;
}

void DoubleContainer_free(DoubleContainer* const dc) {
  Mem_free(dc);
}

double DoubleContainer_get(const DoubleContainer* dc) {
  return *(const double*) dc;
}
