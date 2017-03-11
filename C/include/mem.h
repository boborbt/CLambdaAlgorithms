#pragma once

#include <stdlib.h>

// Mem provides alternatives to the POSIX allocation functions
// that reverts to the standard implementation (with no penality costs)
// if MEM_DEBUG_ON macro si set to 0, while allowing to get debugging
// memory information if the macro is set to 1.
//
// If MEM_DEBUG_ON is set, MEM_VERBOSE controls the level of verbosity
// of the memory functions. If it is set to 1 each alloc/dealloc function
// will output the caller site and the pointer to the memory being
// alloced/dealloced. Otherwise nothing is output.
// Notice that the script parse_mem_logs.rb parses the memory debug output
// signaling the pointers that have not a matching number of allocations
// and deallocations along with the information needed to search for the
// place where the pointer has been originally alloced.
//
// Mem_stats() returns the MemStats structure containing the total size
// of memory that have been alloced and the total size of memory that has
// been dealloced.
//
// **IMPORTANT**: This library is not thread-safe. In particular there
//   is no mutex lock protecting the access to the memory statistics. In
//   threaded environment the results of MemStats are not to be trusted.

typedef struct {
  size_t alloced_memory;
  size_t freed_memory;
} MemStats;

// Macro controlling the behavior of the library. If it is set to
// 0 all functions revert to the POSIX standard ones.
#define MEM_DEBUG_ON 1

// Macro controlling the level of verbosity. Set it to 1 to make each
// function to report information about their callings.
#define MEM_VERBOSE 0


#if MEM_DEBUG_ON==1
  // These macros define the main interface to the library.

  // void* Mem_alloc(size_t size): to be used in place of malloc(.)
  #define Mem_alloc(size) Mem_alloc_ext((size), __FILE__, __LINE__)

  // void* Mem_realloc(void* ptr, size_t size): to be used in place of realloc(.,.)
  #define Mem_realloc(ptr,size) Mem_realloc_ext((ptr),(size), __FILE__, __LINE__)

  // void* Mem_calloc(size_t count, size_t size): to be used in place of calloc(.,.)
  #define Mem_calloc(count,size) Mem_calloc_ext((count), (size), __FILE__, __LINE__)

  // void Mem_free(void* ptr): to be used in place of free(.)
  #define Mem_free(ptr) Mem_free_ext((ptr), __FILE__, __LINE__)

  // char* Mem_strdup(char* str): to be used in place of strdup(.)
  #define Mem_strdup(str) Mem_strdup_ext((str), __FILE__, __LINE__)

  // These are the actual functions that implement the library behavior
  // The user of the library should not use them directly, since these will
  // break the user program when MEM_DEBUG_ON is set to 0 (and the interface
  // defined above is much more convenient to use).
  void* Mem_alloc_ext(size_t size, char* file, size_t lineno);
  void* Mem_realloc_ext(void*, size_t size, char* file, size_t lineno);
  void* Mem_calloc_ext(size_t count, size_t size, char* file, size_t lineno);
  void  Mem_free_ext(void*, char* file, size_t lineno);
  char* Mem_strdup_ext(const char*, char* file, size_t lineno);
#else
  #define Mem_alloc(size) malloc(size)
  #define Mem_realloc(ptr,size) realloc(ptr,size)
  #define Mem_calloc(count,size) calloc(count,size)
  #define Mem_free(ptr) free(ptr)
  #define Mem_strdup(str) strdup(str)
#endif

// Checks if the size of alloced/dealloced memory matches. If the check
// fails it reports the size of the memory alloced and dealloced to the user
// using printf(.)
void Mem_check_and_report(void);

// Returns the current memory statistics
MemStats Mem_stats(void);

// Returns 1 if all alloced memory has been dealloced
int Mem_all_freed(void);
