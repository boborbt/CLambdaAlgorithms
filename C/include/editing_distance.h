#pragma once

// Returns the editing distance between string1 and string2 or LONG_MAX if the distance
// is larger than the given bound. 
// The editing distance implemented here is the number of insertion, deletion or replacements 
// necessary to transform string2 into string1.

long editing_distance(const char* string1, const char* string2, long bound);
