#pragma once

// Returns the editing distance between string1 and string2 or MAX_INT if the distance
// is larger than the given bound. 
// The editing distance implemented here is the number of insertion, deletion or replacements 
// necessary to transform string2 into string1.
// bound == (unsigned long) -1 is used internally and should never be specified by the user
// if bound is set to (unsigned long) -2, is equivalent to not using the bound
unsigned long editing_distance(const char* string1, const char* string2, unsigned long bound);
