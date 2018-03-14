#pragma once

#include <stdio.h>

#include "iterator.h"

// This is a convenience macro to dereference the objects returned by
// iterations started using Number_it
#define NUM(a) (*(unsigned long*) (a))

// Creates a number iterator. The iterator will iterate from 0 to the given integer.
Iterator Number_it(unsigned long);

// Creates a file iterator. The iterator will iterate over all lines of the file (one lines
// per iteration step) up to the end of the file.
Iterator TextFile_it(const char* filename, char delimiter);
