#pragma once

#include <stdlib.h>

typedef struct {
  size_t alloced_memory;
  size_t freed_memory;
} MemStats;

void* Mem_alloc(size_t size);
void* Mem_realloc(void*, size_t size);
void* Mem_calloc(size_t count, size_t size);
void  Mem_free(void*);

MemStats Mem_stats(void);
int Mem_all_freed(void);
