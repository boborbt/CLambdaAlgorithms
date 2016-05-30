#pragma once

#include "dictionary.h"

// PrintTime is a little utility helping in measuring and printing the execution
// time of algorithms. In addition to printing what the user requires to
// stdout, it saves timing information into a file in YAML format.
// If the given file does not exists PrintTime creates it, otherwise it
// appends to it. In any case PrintTime guarantees that savings are atomic.
// The saving is a YAML dictionary having a number of key/value pairs at the
// beginning. These entries are the entry "header" and can be added to the
// saving using the PrintTime_add_header function.
// After the header an entry called "data" is added to the entry. The contents
// of this entry is determined by the callings to PrintTime_print, each entry
// will have the given label as key and the time spent in the block as value.

// PrintTime opaque type
typedef struct _PrintTime* PrintTime;

// Creates a new PrintTime object set to write YAML results on a file named
// out_file.
PrintTime PrintTime_new(const char* out_file);

// Frees the PrintTime data structure.
void PrintTime_free(PrintTime);

// Adds an header to the YAML saving.
void PrintTime_add_header(PrintTime pt, const char* key, const char* value);

// Adds a timing of the given block with a key given by "label". The block can
// itself print necessary information to stdout.
double PrintTime_print(PrintTime pt, char* label, void(^fun)());

// Saves the printing information on disk. The writing is performed atomically.
void PrintTime_save(PrintTime pt);
