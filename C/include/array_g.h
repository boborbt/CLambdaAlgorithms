#pragma once

#include <stdlib.h>

// Returns a pointer to (size * pos) bytes after mem
void* at_g(void* mem, size_t pos, size_t size);

// Copies size bytes from src to dst
void cp_g(void* dst, const void* src, size_t size);

// Swaps size bytes between the memory pointed by p1 and by p2
void swap_g(void* p1, void* p2, size_t size);
