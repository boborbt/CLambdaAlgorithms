#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "editing_distance.h"

typedef struct {
  unsigned long* matrix;
  unsigned long len1;
  unsigned long len2;
} EDMemo;


static unsigned long EDMemo_get(EDMemo* memo, unsigned long i, unsigned long j) {
  return memo->matrix[i * (memo->len2 + 1) + j];
}

static void EDMemo_set(EDMemo* memo, unsigned long i, unsigned long j, unsigned long val) {
  memo->matrix[i * (memo->len2 + 1) + j] = val;
}


__attribute__((unused)) static void EDMemo_print(EDMemo* memo) {
  for(unsigned long i = 0; i < memo->len1 + 1; ++i) {
    for(unsigned long j = 0; j < memo->len2 + 1; ++j) {
      unsigned long value = EDMemo_get(memo, i, j);
      printf("%2d\t", value == ULONG_MAX ? -1 : (int) value);
    }

    printf("\n");
  }
}


static unsigned long EDMemo_memoize(EDMemo* memo, unsigned long i, unsigned long j, unsigned long (^compute_val)(void) ) {
  unsigned long result = EDMemo_get(memo, i, j);
  if(result == ULONG_MAX) {
    result = compute_val();
    EDMemo_set(memo, i, j, result);
  }

  return result;
}

static EDMemo* EDMemo_new(unsigned long len1, unsigned long len2) {
  __block EDMemo* memo = (EDMemo*) malloc(sizeof(EDMemo));
  memo->len1 = len1;
  memo->len2 = len2;
  memo->matrix = (unsigned long*) malloc( (len1 + 1) * (len2 + 1) * sizeof(unsigned long) );

  for(unsigned long i=0; i < len1 + 1; ++i) {
    for(unsigned long j=0; j < len2 + 1; ++j) {
      EDMemo_set(memo,i,j,ULONG_MAX);
    }
  }

  return memo;
}

static void EDMemo_free(EDMemo* memo) {
  free(memo->matrix);
  free(memo);
}


static unsigned long min(unsigned long x, unsigned long y) {
  if(x<y) {
    return x;
  } else {
    return y;
  }
}

// static unsigned long editing_distance_dummy(const char* string1, const char* string2) {
//   if(string1[0] == '\0') {
//     return strlen(string2);
//   }
//
//   if(string2[0] == '\0') {
//     return strlen(string1);
//   }
//
//   unsigned long del_c = editing_distance_dummy(string1, string2+1) + 1;
//   unsigned long copy_c = editing_distance_dummy(string1+1, string2) + 1;
//   if(string1[0] == string2[0]) {
//     return min(del_c, min(copy_c, editing_distance_dummy(string1+1, string2+1)));
//   } else {
//     return min(del_c, copy_c);
//   }
// }

static unsigned long editing_distance_with_memo(EDMemo* memo, const char* string1, const char* string2, unsigned long i, unsigned long j) {
  if(i==memo->len1) {
    return memo->len2 - j;
  }

  if(j==memo->len2) {
    return memo->len1 - i;
  }

  unsigned long cost_with_deletion = EDMemo_memoize(memo, i, j+1, ^(void) {
    return editing_distance_with_memo(memo, string1, string2, i, j+1);
  }) + 1;

  unsigned long cost_with_insertion = EDMemo_memoize(memo, i+1, j, ^(void) {
    return editing_distance_with_memo(memo, string1, string2, i+1, j);
  }) + 1;


  unsigned long cost_with_match =
    (string1[i] != string2[j]) ?
      INT_MAX :
      EDMemo_memoize(memo, i+1, j+1, ^(void) {
        return editing_distance_with_memo(memo, string1, string2, i+1, j+1);
      });

  return min(cost_with_insertion, min(cost_with_deletion, cost_with_match));
}


unsigned long editing_distance(const char* string1, const char* string2) {
  EDMemo* memo = EDMemo_new(strlen(string1), strlen(string2));
  unsigned long result = editing_distance_with_memo(memo, string1, string2, 0, 0);
  EDMemo_free(memo);

  return result;
}
