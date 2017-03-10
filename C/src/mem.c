#include "mem.h"
#include <malloc/malloc.h>

static MemStats mem_stats = { 0, 0 };

void* Mem_alloc(size_t size) {
  void* tmp = malloc(size);
  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

void* Mem_realloc(void* ptr, size_t size) {
  mem_stats.freed_memory += malloc_size(ptr);
  void* tmp = realloc(ptr, size);
  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

void* Mem_calloc(size_t count, size_t size) {
  void* tmp = calloc(count, size);
  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}



void  Mem_free(void* ptr) {
  mem_stats.freed_memory += malloc_size(ptr);
  free(ptr);
}


int Mem_all_freed(void) {
  return mem_stats.alloced_memory == mem_stats.freed_memory;
}

MemStats Mem_stats(void) {
  return mem_stats;
}
