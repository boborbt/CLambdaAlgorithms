#pragma once

// DoubleContainer* opaque type
typedef void DoubleContainer;

// Creates a new container for the given double
DoubleContainer* DoubleContainer_new(double value);

// Frees dc.
void DoubleContainer_free(DoubleContainer* const dc);

// Returns the double contained in dc.
double DoubleContainer_get(const DoubleContainer* dc);

// sets the double contained in dc.
void DoubleContainer_set(DoubleContainer* dc, double value);
