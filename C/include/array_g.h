#pragma once

#include <stdlib.h>

// This utility library contains few functions that help in working with
// arrays of generic elements. Given a pointer to the base of the array and
// the size in bytes of the elements contained, at_g returns the element at
// the given pos; cp_g copies from one location to another one in the same array,
// and swap_g swaps two elements.
//
// This functions come handy in implementing general arrays (as in array.h) or
// sorting functions that do not assume anything about the content of the array
// (such as quick_sort_g declared in quick_sort.h).

// Returns a pointer to (size * pos) bytes after mem
void* at_g(void* mem, size_t pos, size_t size);

// Copies size bytes from src to dst
void cp_g(void* dst, const void* src, size_t size);

// Swaps size bytes between the memory pointed by p1 and by p2
void swap_g(void* p1, void* p2, size_t size);
