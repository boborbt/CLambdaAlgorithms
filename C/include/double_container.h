#pragma once

// DoubleContainer opaque type
typedef struct _DoubleContainer* DoubleContainer;

// Creates a new container for the given double
DoubleContainer DoubleContainer_new(double value);

// Frees dc.
void DoubleContainer_free(DoubleContainer const dc);

// Returns the double contained in dc.
double DoubleContainer_get(const DoubleContainer dc);
