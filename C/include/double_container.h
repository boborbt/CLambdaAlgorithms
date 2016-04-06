#pragma once

typedef struct _DoubleContainer* DoubleContainer;

DoubleContainer DoubleContainer_new(double value);
void DoubleContainer_free(DoubleContainer const dc);
double DoubleContainer_get(const DoubleContainer dc);
