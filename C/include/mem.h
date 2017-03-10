#pragma once

#include <stdlib.h>

typedef struct {
  size_t alloced_memory;
  size_t freed_memory;
} MemStats;

#define Mem_alloc(size) Mem_alloc_ext((size), __FILE__, __LINE__)
#define Mem_realloc(ptr,size) Mem_realloc_ext((ptr),(size), __FILE__, __LINE__)
#define Mem_calloc(count,size) Mem_calloc_ext((count), (size), __FILE__, __LINE__)
#define  Mem_free(ptr) Mem_free_ext((ptr), __FILE__, __LINE__)

void* Mem_alloc_ext(size_t size, char* file, size_t lineno);
void* Mem_realloc_ext(void*, size_t size, char* file, size_t lineno);
void* Mem_calloc_ext(size_t count, size_t size, char* file, size_t lineno);
void  Mem_free_ext(void*, char* file, size_t lineno);



MemStats Mem_stats(void);
int Mem_all_freed(void);
