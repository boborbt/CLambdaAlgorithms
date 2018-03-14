#include <malloc/malloc.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "mem.h"
#include "macros.h"
#include "errors.h"


static MemStats mem_stats = { 0, 0 };

#if MEM_DEBUG_ON == 1

#if MEM_VERBOSE == 1

void* Mem_alloc_ext(size_t size, char* file, size_t line) {
  void* tmp = malloc(size);
  if(tmp == NULL) {
    fprintf(stderr, "Error allocing memory. Reason: %s", strerror(errno));
    exit(ERROR_GENERIC);
  }

  printf("Mem_alloc ptr: %p file: %s line %ld\n", tmp, file, line);

  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#else

void* Mem_alloc_ext(size_t size, char* UNUSED(file), size_t UNUSED(line)) {
  void* tmp = malloc(size);
  if(tmp == NULL) {
    fprintf(stderr, "Error allocing memory. Reason: %s", strerror(errno));
    exit(ERROR_GENERIC);
  }
  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#endif



#if MEM_VERBOSE == 1

void* Mem_realloc_ext(void* ptr, size_t size, char* file, size_t line) {
  mem_stats.freed_memory += malloc_size(ptr);
  void* tmp = realloc(ptr, size);
  if(tmp == NULL) {
    fprintf(stderr, "Error allocing memory. Reason: %s", strerror(errno));
    exit(ERROR_GENERIC);
  }

  printf("Mem_alloc old ptr: %p new_ptr: %p file: %s line %ld\n", ptr, tmp, file, line);

  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#else

void* Mem_realloc_ext(void* ptr, size_t size, char* UNUSED(file), size_t UNUSED(line)) {
  mem_stats.freed_memory += malloc_size(ptr);
  void* tmp = realloc(ptr, size);
  if(tmp == NULL) {
    fprintf(stderr, "Error allocing memory. Reason: %s", strerror(errno));
    exit(ERROR_GENERIC);
  }
  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#endif

#if MEM_VERBOSE == 1

void* Mem_calloc_ext(size_t count, size_t size, char* file, size_t line) {
  void* tmp = calloc(count, size);
  if(tmp == NULL) {
    fprintf(stderr, "Error allocing memory. Reason: %s", strerror(errno));
    exit(ERROR_GENERIC);
  }

  printf("Mem_clloc ptr: %p file: %s line %ld\n", tmp, file, line);

  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#else

void* Mem_calloc_ext(size_t count, size_t size, char* UNUSED(file), size_t UNUSED(line)) {
  void* tmp = calloc(count, size);
  if(tmp == NULL) {
    fprintf(stderr, "Error allocing memory. Reason: %s", strerror(errno));
    exit(ERROR_GENERIC);
  }

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


#if MEM_VERBOSE == 1

char* Mem_strdup_ext(const char* str, char* file, size_t line) {
  char* tmp = strdup(str);
  if(tmp == NULL) {
    fprintf(stderr, "Error duplicating memory. Reason: %s", strerror(errno));
    exit(ERROR_GENERIC);
  }

  printf("Mem_strdup ptr: %p file: %s line %ld\n", (void*)tmp, file, line);

  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#else

char* Mem_strdup_ext(const char* str, char* UNUSED(file), size_t UNUSED(line)) {
  char* tmp = strdup(str);
  if(tmp == NULL) {
    fprintf(stderr, "Error duplicating memory. Reason: %s", strerror(errno));
    exit(ERROR_GENERIC);
  }

  mem_stats.alloced_memory += malloc_size(tmp);
  return tmp;
}

#endif

#endif


int Mem_all_freed(void) {
  return mem_stats.alloced_memory == mem_stats.freed_memory;
}

MemStats Mem_stats(void) {
  return mem_stats;
}

void Mem_check_and_report(void) {
  MemStats stats = Mem_stats();
  if(!Mem_all_freed()) {
    printf("\nLeaked memory detected\n");
    printf("alloced: %ld\n", stats.alloced_memory);
    printf("freed: %ld\n", stats.freed_memory);
    printf("difference: %ld\n", stats.alloced_memory - stats.freed_memory);
  }
}
