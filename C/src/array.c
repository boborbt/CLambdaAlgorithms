#include "array.h"
#include <string.h>
#include <stdio.h>
#include "array_g.h"


struct _Array {
  void* carray;
  size_t capacity;
  size_t size;
  size_t elem_size;
};

struct _ArrayIterator {
  Array array;
  size_t current_index;
};


// Constructors
Array Array_new(size_t capacity, size_t elem_size) {
  Array array = (Array) malloc(sizeof(struct _Array));
  array->carray = (void*) malloc(elem_size * capacity);
  array->size = 0;
  array->capacity = capacity;
  array->elem_size = elem_size;
  return array;
}


Array Array_new_by_copying_carray(void* src, size_t size, size_t elem_size ) {
  size_t capacity = size + 100;
  Array array = (Array) malloc(sizeof(struct _Array));
  array->carray = malloc(capacity * elem_size);
  array->capacity = capacity;
  array->size = size;
  array->elem_size = elem_size;

  memcpy(array->carray, src, size * elem_size);
  return array;
}

// Destructor
void Array_free(Array array) {
  if(array) {
    if(array->carray) {
      free(array->carray);
    }

    free(array);
  }
}

// Accessors
void* Array_at(Array array, size_t index) {
  return *(void**)at_g(array->carray, index, array->elem_size);
}

void* Array_carray(Array array) {
  return array->carray;
}

size_t Array_size(Array array) {
  return array->size;
}

size_t Array_capacity(Array array) {
  return array->capacity;
}

// Setters

static void Array_realloc(Array array) {
  array->capacity *= 2;
  array->carray = realloc(array->carray, array->capacity * array->elem_size);
}

void Array_set_size(Array array, size_t new_size) {
  while(array->capacity < new_size) {
    Array_realloc(array);
  }

  array->size = new_size;
}

void* Array_set(Array array, size_t index, void* elem) {
  if(index >= array->size) {
    fprintf(stderr, "Array index (%ld) out of bound in array of size (%ld)\n", index, array->size);
    exit(1);
  }
  cp_g(at_g(array->carray, index, array->elem_size), elem, array->elem_size);
  return elem;
}

void Array_add(Array array, void* elem) {
  if(array->size >= array->capacity) {
    Array_realloc(array);
  }

  Array_set(array, array->size++, elem);
}

void Array_insert(Array array, size_t index, void* elem) {
  Array_add(array, NULL);
  memmove(at_g(array->carray, index+1, array->elem_size),
          at_g(array->carray, index, array->elem_size),
          (array->size - index)*array->elem_size);
  Array_set(array, index, elem);
}

void Array_remove(Array array, size_t index) {
  memmove(at_g(array->carray, index+1, array->elem_size),
          at_g(array->carray, index, array->elem_size),
          (array->size - (index+1))*array->elem_size);
  array->size -= 1;
}

// Iterator
ArrayIterator ArrayIterator_new(Array array) {
  ArrayIterator iterator = (ArrayIterator) malloc(sizeof(struct _ArrayIterator));
  iterator->array = array;
  iterator->current_index = 0;

  return iterator;
}

void ArrayIterator_free(ArrayIterator iterator) {
  free(iterator);
}

// Move the iterator to the next element. Do nothing if it is already past the
// end of the container.
void ArrayIterator_next(ArrayIterator it) {
  it->current_index += 1;
}

// Returns 1 if the iterator is past the end of the container (i.e., if
// ArrayIterator_get would return a sensible result), 0 otherwise.
int ArrayIterator_end(ArrayIterator it) {
  return it->current_index >= it->array->size;
}

// Returns the element currently pointed by the iterator
void* ArrayIterator_get(ArrayIterator it) {
  return Array_at(it->array, it->current_index);
}

void foreach_array_elem(Array array, void (^callback)(void*)) {
  ArrayIterator it = ArrayIterator_new(array);
  while(!ArrayIterator_end(it)) {
    callback(ArrayIterator_get(it));
    ArrayIterator_next(it);
  }
  ArrayIterator_free(it);
}

void foreach_array_elem_with_index(Array array, void (^callback)(void*, size_t)) {
  ArrayIterator it = ArrayIterator_new(array);
  size_t count = 0;
  while(!ArrayIterator_end(it)) {
    callback(ArrayIterator_get(it), count++);
    ArrayIterator_next(it);
  }
  ArrayIterator_free(it);
}
