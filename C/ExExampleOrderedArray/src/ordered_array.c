#include <stdlib.h>
#include <assert.h>

#include "ordered_array.h"

// --------------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------------

#define ORDERED_ARRAY_INITIAL_CAPACITY 1024

// --------------------------------------------------------------------------------
// Types and data structures
// --------------------------------------------------------------------------------

struct _OrderedArray {
  void** carray;
  long capacity;
  long count;

  CompareCallback compare;
};

typedef int (*CompareCallback)(void*, void*);

// --------------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------------

static void OrderedArray_resize(OrderedArray* ordered_array) {
  ordered_array->capacity *= 2;

  ordered_array->carray = (void**) realloc(ordered_array->carray, sizeof(void*) * (size_t) ordered_array->capacity);
  assert(ordered_array->carray != NULL);
}

// --------------------------------------------------------------------------------
// Public interface
// --------------------------------------------------------------------------------

OrderedArray* OrderedArray_new(CompareCallback compare) {
  OrderedArray* result = (OrderedArray*) malloc(sizeof(OrderedArray));

  result->capacity = ORDERED_ARRAY_INITIAL_CAPACITY;
  result->carray = (void**) malloc(sizeof(void*) * (size_t) result->capacity);
  assert(result->carray != NULL);

  result->count = 0;
  result->compare = compare;

  return result;
}

void OrderedArray_free(OrderedArray* ordered_array) {
  free(ordered_array->carray);
  free(ordered_array);
}

long OrderedArray_count(OrderedArray* ordered_array) {
  return ordered_array->count;
}



void OrderedArray_add(OrderedArray* ordered_array, void* obj) {
  if(ordered_array->count == ordered_array->capacity) {
    OrderedArray_resize(ordered_array);
  }

  ordered_array->count += 1;
  long i;

  // invariant: carray[i+1] is the spot to be filled in
  for(i = ordered_array->count - 2;
      i>=0 && ordered_array->compare(obj, ordered_array->carray[i]) < 0;
      i--) {
      ordered_array->carray[i+1] = ordered_array->carray[i];
  }

  ordered_array->carray[i+1] = obj;
}

void OrderedArray_remove_at(OrderedArray* ordered_array, long index) {
  assert(index >= 0 && index < ordered_array->count);

  for(long i = index; i<ordered_array->count - 1; ++i) {
    ordered_array->carray[i] = ordered_array->carray[i+1];
  }

  ordered_array->count--;
}

void* OrderedArray_at(OrderedArray* ordered_array, long index) {
  assert(index >= 0 && index < ordered_array->count);

  return ordered_array->carray[index];
}
