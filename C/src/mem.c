#include "mem.h"
#include "macros.h"
#include <malloc/malloc.h>
#include <stdio.h>

#define MEM_VERBOSE 0

static MemStats mem_stats = { 0, 0 };

#if MEM_VERBOSE == 1

void* Mem_alloc_ext(size_t size, char* file, size_t line) {
  void* tmp = malloc(size);

  printf("Mem_alloc ptr: %p file: %s line %ld\n", tmp, file, line);

  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#else

void* Mem_alloc_ext(size_t size, char* UNUSED(file), size_t UNUSED(line)) {
  void* tmp = malloc(size);
  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#endif



#if MEM_VERBOSE == 1

void* Mem_realloc_ext(void* ptr, size_t size, char* file, size_t line) {
  mem_stats.freed_memory += malloc_size(ptr);
  void* tmp = realloc(ptr, size);

  printf("Mem_alloc old ptr: %p new_ptr: %p file: %s line %ld\n", ptr, tmp, file, line);

  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#else

void* Mem_realloc_ext(void* ptr, size_t size, char* UNUSED(file), size_t UNUSED(line)) {
  mem_stats.freed_memory += malloc_size(ptr);
  void* tmp = realloc(ptr, size);

  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#endif

#if MEM_VERBOSE == 1

void* Mem_calloc_ext(size_t count, size_t size, char* file, size_t line) {
  void* tmp = calloc(count, size);

  printf("Mem_clloc ptr: %p file: %s line %ld\n", tmp, file, line);

  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#else

void* Mem_calloc_ext(size_t count, size_t size, char* UNUSED(file), size_t UNUSED(line)) {
  void* tmp = calloc(count, size);

  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#endif


#if MEM_VERBOSE == 1

void  Mem_free_ext(void* ptr, char* file, size_t line) {

  printf("Mem_free ptr: %p file: %s line %ld\n", ptr, file, line);

  mem_stats.freed_memory += malloc_size(ptr);
  free(ptr);
}

#else

void  Mem_free_ext(void* ptr, char* UNUSED(file), size_t UNUSED(line)) {

  mem_stats.freed_memory += malloc_size(ptr);
  free(ptr);
}

#endif



int Mem_all_freed(void) {
  return mem_stats.alloced_memory == mem_stats.freed_memory;
}

MemStats Mem_stats(void) {
  return mem_stats;
}
