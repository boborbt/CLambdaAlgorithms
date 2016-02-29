#include <stdlib.h>
#include "double_container.h"

typedef struct _DoubleContainer {
  double value;
}* DoubleContainer;

DoubleContainer DoubleContainer_new(double value) {
  DoubleContainer result = (DoubleContainer) malloc(sizeof(struct _DoubleContainer));
  result->value = value;
  return result;
}
void DoubleContainer_free(DoubleContainer dc) {
  free(dc);
}

double DoubleContainer_get(DoubleContainer dc) {
  return dc->value;
}
