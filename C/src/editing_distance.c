#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include "mem.h"
#include "editing_distance.h"
#include "iterator_functions.h"
#include "basic_iterators.h"

//
// Implementation of the EditingDistance measure
//
// The starting point on which the implementation is based is the
// recursive function `editing_distance_dummy`. Which works, but unfortunately
// it has an exponential cost. The implementation adds memoization to this
// algorithm bringing the cost to linear.
//


// Min function
static long min(long x, long y) {
  if(x<y) {
    return x;
  } else {
    return y;
  }
}

// Recursive function to compute the editing distance between string1 and string2.
__attribute__((unused)) static long editing_distance_dummy(const char* string1, const char* string2) {
  if(string1[0] == '\0') {
    return (long) strlen(string2);
  }

  if(string2[0] == '\0') {
    return (long) strlen(string1);
  }

  long del_c = editing_distance_dummy(string1, string2+1) + 1;
  long copy_c = editing_distance_dummy(string1+1, string2) + 1;
  if(string1[0] == string2[0]) {
    return min(del_c, min(copy_c, editing_distance_dummy(string1+1, string2+1)));
  } else {
    return min(del_c, copy_c);
  }
}


//  Definition of the structure used to memoize intermediate values
typedef struct {
  long* matrix;
  long len1;
  long len2;
} EDMemo;

// Return the value in position i,j
static long EDMemo_get(EDMemo* memo, long i, long j) {
  assert(i < memo->len1);
  assert(j < memo->len2);
  return memo->matrix[i * memo->len2 + j];
}

// Set the value in position i,j to the content of `val`
static void EDMemo_set(EDMemo* memo, long i, long j, long val) {
  assert(i < memo->len1);
  assert(j < memo->len2);
  memo->matrix[i * memo->len2 + j] = val;
}

// Returns the first length used to initialize the data structure.
// Note: internally the length is not the same as the one used in the call
//    to EDMemo_new, *always* use this method when retrieving the length from
//    the perspective of the data structure user, *always* use memo->len1 when
//    dealing with the internal storage memo->matrix.
static long EDMemo_len1(EDMemo* memo) {
  return memo->len1 - 1;
}

// Returns the second length used to initialize the data structure.
// Note: internally the length is not the same as the one used in the call
//    to EDMemo_new, *always* use this method when retrieving the length from
//    the perspective of the data structure user, *always* use memo->len2 when
//    dealing with the internal storage memo->matrix.
static long EDMemo_len2(EDMemo* memo) {
  return memo->len2 - 1;
}

// Debugging function: it prints the content of the memoization matrix
__attribute__((unused)) static void EDMemo_print(EDMemo* memo) {
  for_each(Number_it((unsigned long) memo->len1), ^(void* i) {

    for_each(Number_it((unsigned long)memo->len2), ^(void* j) {
      long value = EDMemo_get(memo, (long) NUM(i), (long) NUM(j));
      printf("%2ld\t", value);
    });

    printf("\n");
  });
}

// Memoization operator
// If element in position i,j has already been set to a sensible value, it just
// returns that value. Otherwise it calls `compute_val` to compute a new value
// for those coordinates, memorize that value in position i,j and returns the computed value.

static long EDMemo_memoize(EDMemo* memo, long i, long j, long (^compute_val)(void) ) {
  long result = EDMemo_get(memo, i, j);
  if(result == -1) {
    result = compute_val();

    if(result != LONG_MAX) {
      EDMemo_set(memo, i, j, result);
    }
  }

  return result;
}

// Creates a new memoization structure
// Note: the given len1 and len2 will not be the same as memo->len1 and memo->len2
//    use EDMemo_len1() and EDMemo_len2() to retrieve the lengths passed to this function.
static EDMemo* EDMemo_new(long len1, long len2) {
  __block EDMemo* memo = (EDMemo*) Mem_alloc(sizeof(EDMemo));
  memo->len1 = len1 + 1;
  memo->len2 = len2 + 1;
  memo->matrix = (long*) Mem_alloc( ((unsigned long) memo->len1) * ((unsigned long) memo->len2) * sizeof(long) );

  for_each(Number_it((unsigned long) memo->len1), ^(void* i) {
    for_each(Number_it((unsigned long)memo->len2), ^(void* j) {
      EDMemo_set(memo,  NUM(i),  NUM(j), -1);
    });
  });

  return memo;
}

// Free the memoization structure
static void EDMemo_free(EDMemo* memo) {
  Mem_free(memo->matrix);
  Mem_free(memo);
}


__attribute__((unused)) 
static void log_ed(const char* string1, const char* string2, long i, long j, long bound, long distance) {
  const char* s1 = &string1[i];
  const char* s2 = &string2[j];
  printf("Ed distnace (%ld, %ld): %s - %s --> %ld (%ld)\n", i, j, s1, s2, distance, bound);
}

// Uses memoization to compute the editing distance between string1 and string2 assuming
// that someone already took care of all string1[0..i] and string2[0..j].

static long editing_distance_with_memo(EDMemo* memo, const char* string1, const char* string2, long i, long j, long bound) {
  // #define LOG_ED(dist) log_ed(string1, string2, i, j, bound, (dist))
  #define LOG_ED(dist)

  // if(i == 0 && j==0) {
  //   printf("\n\nEd distance [%s -- %s]\n", string1, string2);
  // }


  if(bound < 0) {
    LOG_ED(LONG_MAX);
    return LONG_MAX;
  }

  if(string1[i] == '\0') {
    long result = EDMemo_len2(memo) - j;
    result = result <= bound ? result : LONG_MAX;
    LOG_ED(result);
    return result;
  }

  if(string2[j] == '\0') {
    long result = EDMemo_len1(memo) - i;
    result = result <= bound ? result : LONG_MAX;
    LOG_ED(result);
    return result;
  }

  long cost_with_deletion = EDMemo_memoize(memo, i, j+1, ^(void) {
    return editing_distance_with_memo(memo, string1, string2, i, j+1, bound-1);
  });
  cost_with_deletion = (cost_with_deletion == LONG_MAX ? LONG_MAX : (cost_with_deletion + 1));
  bound = min(cost_with_deletion, bound);

  long cost_with_insertion = EDMemo_memoize(memo, i+1, j, ^(void) {
    return editing_distance_with_memo(memo, string1, string2, i+1, j, bound-1);
  });
  cost_with_insertion = (cost_with_insertion == LONG_MAX ? LONG_MAX : (cost_with_insertion + 1));
  bound = min(cost_with_insertion, bound);

  long cost_with_replacement = EDMemo_memoize(memo, i+1, j+1, ^(void) {
    return editing_distance_with_memo(memo, string1, string2, i+1, j+1, bound-1);
  });
  cost_with_replacement = (cost_with_replacement == LONG_MAX ? LONG_MAX : (cost_with_replacement + 1));
  bound = min(cost_with_replacement, bound);


  long cost_with_match = LONG_MAX;
  if(string1[i] == string2[j]) {
    cost_with_match = EDMemo_memoize(memo, i+1, j+1, ^(void) {
      return editing_distance_with_memo(memo, string1, string2, i+1, j+1, bound);
    });
  }

  long result = min(cost_with_deletion,
                min(cost_with_insertion,
                min(cost_with_replacement, cost_with_match)));


  // if(bound<result) {
  //   return LONG_MAX;
  // }
  LOG_ED(result);
  return result;
  #undef LOG_ED
}

// The code below is equivalent and cleaner w.r.t. the code above. Unfortunately is slightly slower.
// 
// static long editing_distance_with_memo(EDMemo* memo, const char* string1, const char* string2, long i, long j) {
//
//   return EDMemo_memoize(memo, i, j, ^{
//     if(string1[i] == '\0') {
//       return EDMemo_len2(memo) - j;
//     }
//
//     if(string2[j] == '\0') {
//       return EDMemo_len1(memo) - i;
//     }
//
//     long cost_with_deletion = editing_distance_with_memo(memo, string1, string2, i, j+1) + 1;
//     long cost_with_insertion =  editing_distance_with_memo(memo, string1, string2, i+1, j) + 1;
//     long cost_with_replacement = editing_distance_with_memo(memo, string1, string2, i+1, j+1) + 1;
//
//     long cost_with_match = LONG_MAX;
//     if(string1[i] == string2[j]) {
//       cost_with_match = editing_distance_with_memo(memo, string1, string2, i+1, j+1);
//     }
//
//     return min(cost_with_replacement, min(cost_with_insertion, min(cost_with_deletion, cost_with_match)));
//   });
// }


// Returns the editing distance between string1 and string2
long editing_distance(const char* string1, const char* string2, long bound) {
  EDMemo* memo = EDMemo_new((long) strlen(string1), (long) strlen(string2));
  long result = editing_distance_with_memo(memo, string1, string2, 0, 0, bound);
  EDMemo_free(memo);

  return result;
}
