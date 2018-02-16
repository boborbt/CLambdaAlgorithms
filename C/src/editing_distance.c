#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include "editing_distance.h"

//
// Implementation of the EditingDistance measure
//
// The starting point on which the implementation is based is the
// recursive function `editing_distance_dummy`. Which works, but unfortunately
// it has an exponential cost. The implementation adds memoization to this
// algorithm bringing the cost to linear.
//


// Min function
static unsigned long min(unsigned long x, unsigned long y) {
  if(x<y) {
    return x;
  } else {
    return y;
  }
}

// Recursive function to compute the editing distance between string1 and string2.
__attribute__((unused)) static unsigned long editing_distance_dummy(const char* string1, const char* string2) {
  if(string1[0] == '\0') {
    return strlen(string2);
  }

  if(string2[0] == '\0') {
    return strlen(string1);
  }

  unsigned long del_c = editing_distance_dummy(string1, string2+1) + 1;
  unsigned long copy_c = editing_distance_dummy(string1+1, string2) + 1;
  if(string1[0] == string2[0]) {
    return min(del_c, min(copy_c, editing_distance_dummy(string1+1, string2+1)));
  } else {
    return min(del_c, copy_c);
  }
}


//  Definition of the structure used to memoize intermediate values
typedef struct {
  unsigned long* matrix;
  unsigned long len1;
  unsigned long len2;
} EDMemo;

// Return the value in position i,j
static unsigned long EDMemo_get(EDMemo* memo, unsigned long i, unsigned long j) {
  assert(i < memo->len1);
  assert(j < memo->len2);
  return memo->matrix[i * memo->len2 + j];
}

// Set the value in position i,j to the content of `val`
static void EDMemo_set(EDMemo* memo, unsigned long i, unsigned long j, unsigned long val) {
  assert(i < memo->len1);
  assert(j < memo->len2);
  memo->matrix[i * memo->len2 + j] = val;
}

// Returns the first length used to initialize the data structure.
// Note: internally the length is not the same as the one used in the call
//    to EDMemo_new, *always* use this method when retrieving the length from
//    the perspective of the data structure user, *always* use memo->len1 when
//    dealing with the internal storage memo->matrix.
static unsigned long EDMemo_len1(EDMemo* memo) {
  return memo->len1 - 1;
}

// Returns the second length used to initialize the data structure.
// Note: internally the length is not the same as the one used in the call
//    to EDMemo_new, *always* use this method when retrieving the length from
//    the perspective of the data structure user, *always* use memo->len2 when
//    dealing with the internal storage memo->matrix.
static unsigned long EDMemo_len2(EDMemo* memo) {
  return memo->len2 - 1;
}

// Debugging function: it prints the content of the memoization matrix
__attribute__((unused)) static void EDMemo_print(EDMemo* memo) {
  for(unsigned long i = 0; i < memo->len1; ++i) {
    for(unsigned long j = 0; j < memo->len2; ++j) {
      unsigned long value = EDMemo_get(memo, i, j);
      printf("%2d\t", value == ULONG_MAX ? -1 : (int) value);
    }

    printf("\n");
  }
}

// Memoization operator
// If element in position i,j has already been set to a sensible value, it just
// returns that value. Otherwise it calls `compute_val` to compute a new value
// for those coordinates, memorize that value in position i,j and returns the computed value.

static unsigned long EDMemo_memoize(EDMemo* memo, unsigned long i, unsigned long j, unsigned long (^compute_val)(void) ) {
  unsigned long result = EDMemo_get(memo, i, j);
  if(result == ULONG_MAX) {
    result = compute_val();
    EDMemo_set(memo, i, j, result);
  }

  return result;
}

// Creates a new memoization structure
// Note: the given len1 and len2 will not be the same as memo->len1 and memo->len2
//    use EDMemo_len1() and EDMemo_len2() to retrieve the lengths passed to this function.
static EDMemo* EDMemo_new(unsigned long len1, unsigned long len2) {
  __block EDMemo* memo = (EDMemo*) malloc(sizeof(EDMemo));
  memo->len1 = len1 + 1;
  memo->len2 = len2 + 1;
  memo->matrix = (unsigned long*) malloc( memo->len1 * memo->len2 * sizeof(unsigned long) );

  for(unsigned long i=0; i < memo->len1; ++i) {
    for(unsigned long j=0; j < memo->len2; ++j) {
      EDMemo_set(memo,i,j,ULONG_MAX);
    }
  }

  return memo;
}

// Free the memoization structure
static void EDMemo_free(EDMemo* memo) {
  free(memo->matrix);
  free(memo);
}

// Uses memoization to compute the editing distance between string1 and string2 assuming
// that someone already took care of all string1[0..i] and string2[0..j].
static unsigned long editing_distance_with_memo(EDMemo* memo, const char* string1, const char* string2, unsigned long i, unsigned long j) {
  if(string1[i] == '\0') {
    return EDMemo_len2(memo) - j;
  }

  if(string2[j] == '\0') {
    return EDMemo_len1(memo) - i;
  }

  unsigned long cost_with_deletion = EDMemo_memoize(memo, i, j+1, ^(void) {
    return editing_distance_with_memo(memo, string1, string2, i, j+1);
  }) + 1;

  unsigned long cost_with_insertion = EDMemo_memoize(memo, i+1, j, ^(void) {
    return editing_distance_with_memo(memo, string1, string2, i+1, j);
  }) + 1;


  unsigned long cost_with_match = INT_MAX;
  if(string1[i] == string2[j]) {
    cost_with_match = EDMemo_memoize(memo, i+1, j+1, ^(void) {
      return editing_distance_with_memo(memo, string1, string2, i+1, j+1);
    });
  }

  return min(cost_with_insertion, min(cost_with_deletion, cost_with_match));
}


// Returns the editing distance between string1 and string2
unsigned long editing_distance(const char* string1, const char* string2) {
  EDMemo* memo = EDMemo_new(strlen(string1), strlen(string2));
  unsigned long result = editing_distance_with_memo(memo, string1, string2, 0, 0);
  EDMemo_free(memo);

  return result;
}
