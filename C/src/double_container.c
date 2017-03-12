#include <stdlib.h>
#include "double_container.h"
#include "mem.h"

struct _DoubleContainer {
  double value;
};

DoubleContainer* DoubleContainer_new(double value) {
  DoubleContainer* result = (DoubleContainer*) Mem_alloc(sizeof(struct _DoubleContainer));
  result->value = value;
  return result;
}
void DoubleContainer_free(DoubleContainer* const dc) {
  Mem_free(dc);
}

double DoubleContainer_get(const DoubleContainer* dc) {
  return dc->value;
}
