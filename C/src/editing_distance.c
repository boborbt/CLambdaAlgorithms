#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "editing_distance.h"

typedef struct {
  int* matrix;
  int len1;
  int len2;
} EDMemo;


int EDMemo_get(EDMemo* memo, int i, int j) {
  printf("get(%d,%d)=%d\n", i, j, memo->matrix[i * memo->len1 + j]);
  return memo->matrix[i * memo->len1 + j];
}

void EDMemo_set(EDMemo* memo, int i, int j, int val) {
  printf("set(%d,%d)=%d\n", i, j, val);
  memo->matrix[i * memo->len1 + j] = val;
}

static EDMemo* EDMemo_new(int len1, int len2) {
  __block EDMemo* memo = (EDMemo*) malloc(sizeof(EDMemo));
  memo->len1 = len1;
  memo->len2 = len2;
  memo->matrix = (int*) malloc( (len1 + 1) * (len2 + 1) * sizeof(int) );

  for(int i=0; i < len1 + 1; ++i) {
    for(int j=0; j < len2 + 1; ++j) {
      EDMemo_set(memo,i,j,-2);
    }
  }

  return memo;
}

static void EDMemo_free(EDMemo* memo) {
  free(memo->matrix);
  free(memo);
}


int min(int x, int y) {
  if(x<y) {
    return x;
  } else {
    return y;
  }
}

int editing_distance_dummy(const char* string1, const char* string2) {
  if(strlen(string1) == 0) {
    return strlen(string2);
  }

  if(strlen(string2) == 0) {
    return strlen(string1);
  }

  int del_c = editing_distance_dummy(string1, string2+1) + 1;
  int copy_c = editing_distance_dummy(string1+1, string2) + 1;
  if(string1[0] == string2[0]) {
    return min(del_c, min(copy_c, editing_distance_dummy(string1+1, string2+1)));
  } else {
    return min(del_c, copy_c);
  }
}

int editing_distance_with_memo(EDMemo* memo, const char* string1, const char* string2, int i, int j) {
  if(i==memo->len1) {
    return memo->len2 - j;
  }

  if(j==memo->len2) {
    return memo->len1 - i;
  }

  int cost_with_deletion = EDMemo_get(memo, i, j+1) + 1;
  if(cost_with_deletion < 0) {
    cost_with_deletion = editing_distance_with_memo(memo, string1, string2, i, j+1) + 1;
  }

  int cost_with_insertion = EDMemo_get(memo, i+1, j) + 1;
  if(cost_with_insertion < 0) {
    cost_with_insertion = editing_distance_with_memo(memo, string1, string2, i+1, j) + 1;
  }

  int cost_with_match;
  if(string1[i] == string2[j]) {
     cost_with_match = EDMemo_get(memo, i+1, j+1);
    if( cost_with_match < 0 ) {
      cost_with_match = editing_distance_with_memo(memo, string1, string2, i+1, j+1);
    }
  } else {
    cost_with_match = INT_MAX;
  }

  int result = min(cost_with_insertion, min(cost_with_deletion, cost_with_match));
  EDMemo_set(memo, i, j, result);

  return result;
}


int editing_distance(const char* string1, const char* string2) {
  EDMemo* memo = EDMemo_new(strlen(string1), strlen(string2));

  int result = editing_distance_with_memo(memo, string1, string2, 0, 0);

  EDMemo_free(memo);

  return result;
}

int main() {
  printf("%d\n", editing_distance("portoricano", "puertorico"));
  return 0;
}
