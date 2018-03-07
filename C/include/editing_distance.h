#pragma once

// Returns the editing distance between string1 and string2. The editing distance
// implemented here is the number of insertion or deletion necessary to transform
// string2 into string1.
unsigned long editing_distance(const char* string1, const char* string2);
