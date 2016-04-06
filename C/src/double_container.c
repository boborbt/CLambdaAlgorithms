#include <stdlib.h>
#include "double_container.h"

struct _DoubleContainer {
  double value;
};

DoubleContainer DoubleContainer_new(double value) {
  DoubleContainer result = (DoubleContainer) malloc(sizeof(struct _DoubleContainer));
  result->value = value;
  return result;
}
void DoubleContainer_free(DoubleContainer const dc) {
  free(dc);
}

double DoubleContainer_get(const DoubleContainer dc) {
  return dc->value;
}
