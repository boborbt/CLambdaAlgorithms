#include "array_alt.h"
#include <string.h>
#include <stdio.h>
#include "array_g.h"
#include "quick_sort.h"
#include "errors.h"
#include "mem.h"

struct _ArrayAlt {
  void* carray;
  size_t capacity;
  size_t size;
  size_t elem_size;
};

struct _ArrayAltIterator {
  ArrayAlt* array;
  size_t current_index;
};


// Constructors
ArrayAlt* ArrayAlt_new(size_t capacity, size_t elem_size) {
  ArrayAlt* array = (ArrayAlt*) Mem_alloc(sizeof(struct _ArrayAlt));
  array->carray = (void*) Mem_alloc(elem_size * capacity);
  array->size = 0;
  array->capacity = capacity;
  array->elem_size = elem_size;
  return array;
}


ArrayAlt* ArrayAlt_new_by_copying_carray(void* src, size_t size, size_t elem_size ) {
  size_t capacity = size + 100;
  ArrayAlt* array = (ArrayAlt*) Mem_alloc(sizeof(struct _ArrayAlt));
  array->carray = Mem_alloc(capacity * elem_size);
  array->capacity = capacity;
  array->size = size;
  array->elem_size = elem_size;

  memcpy(array->carray, src, size * elem_size);
  return array;
}

ArrayAlt* ArrayAlt_dup(ArrayAlt* array) {
  return ArrayAlt_new_by_copying_carray(ArrayAlt_carray(array), ArrayAlt_size(array), ArrayAlt_elem_size(array));
}

// Destructor
void ArrayAlt_free(ArrayAlt* array) {
  if(array) {
    if(array->carray) {
      Mem_free(array->carray);
    }

    Mem_free(array);
  }
}

// Accessors
void* ArrayAlt_at(ArrayAlt* array, size_t index) {
  if(index >= ArrayAlt_size(array)) {
    Error_raise(Error_new(ERROR_INDEX_OUT_OF_BOUND, "Index %ld is out of bounds (0,%ld)", index, ArrayAlt_size(array) ));
  }

  return at_g(array->carray, index, array->elem_size);
}

void* ArrayAlt_carray(ArrayAlt* array) {
  return array->carray;
}

size_t ArrayAlt_size(ArrayAlt* array) {
  return array->size;
}

int ArrayAlt_empty(ArrayAlt* array) {
  return ArrayAlt_size(array) == 0;
}

size_t ArrayAlt_capacity(ArrayAlt* array) {
  return array->capacity;
}

size_t ArrayAlt_elem_size(ArrayAlt* array) {
  return array->elem_size;
}

// Setters

static void ArrayAlt_realloc(ArrayAlt* array) {
  array->capacity *= 2;
  array->carray =Mem_realloc(array->carray, array->capacity * array->elem_size);
}

void ArrayAlt_set_size(ArrayAlt* array, size_t new_size) {
  while(array->capacity < new_size) {
    ArrayAlt_realloc(array);
  }

  array->size = new_size;
}

void* ArrayAlt_set(ArrayAlt* array, size_t index, void* elem) {
  if(index >= array->size) {
    Error_raise(Error_new(ERROR_INDEX_OUT_OF_BOUND,"ArrayAlt* index (%ld) out of bound in array of size (%ld)\n", index, array->size));
  }
  cp_g(at_g(array->carray, index, array->elem_size), elem, array->elem_size);
  return elem;
}

void ArrayAlt_add(ArrayAlt* array, void* elem) {
  if(array->size >= array->capacity) {
    ArrayAlt_realloc(array);
  }

  ArrayAlt_set(array, array->size++, elem);
}

void ArrayAlt_insert(ArrayAlt* array, size_t index, void* elem) {
  if(array->size >= array->capacity) {
    ArrayAlt_realloc(array);
  }

  memmove(at_g(array->carray, index+1, array->elem_size),
          at_g(array->carray, index, array->elem_size),
          (array->size - index)*array->elem_size);

  array->size++;
  ArrayAlt_set(array, index, elem);
}

void ArrayAlt_remove(ArrayAlt* array, size_t index) {
  memmove(at_g(array->carray, index, array->elem_size),
          at_g(array->carray, index+1, array->elem_size),
          (array->size - (index+1)) * array->elem_size);
  array->size -= 1;
}

void ArrayAlt_sort(ArrayAlt* array, KIComparator compare) {
  quick_sort_g(ArrayAlt_carray(array), ArrayAlt_size(array), array->elem_size, compare);
}

// Iterator
ArrayAltIterator* ArrayAltIterator_new(ArrayAlt* array) {
  ArrayAltIterator* iterator = (ArrayAltIterator*) Mem_alloc(sizeof(struct _ArrayAltIterator));
  iterator->array = array;
  iterator->current_index = 0;

  return iterator;
}

void ArrayAltIterator_free(ArrayAltIterator* iterator) {
  Mem_free(iterator);
}

// Move the iterator to the next element. Do nothing if it is already past the
// end of the container.
void ArrayAltIterator_next(ArrayAltIterator* it) {
  it->current_index += 1;
}

// Returns 1 if the iterator is past the end of the container (i.e., if
// ArrayAltIterator_get would return a sensible result), 0 otherwise.
int ArrayAltIterator_end(ArrayAltIterator* it) {
  return it->current_index >= it->array->size;
}

// Returns the element currently pointed by the iterator
void* ArrayAltIterator_get(ArrayAltIterator* it) {
  return ArrayAlt_at(it->array, it->current_index);
}

void ArrayAltIterator_to_begin(ArrayAltIterator* it) {
  it->current_index = 0;
}

int ArrayAltIterator_same(ArrayAltIterator* it1, ArrayAltIterator* it2) {
  return  it1->array == it2->array && it1->current_index == it2->current_index;
}

Iterator ArrayAlt_it(ArrayAlt* array)
{
 return Iterator_make(
   array,
   (void* (*)(void*)) ArrayAltIterator_new,
   (void (*)(void*))  ArrayAltIterator_next,
   (void* (*)(void*)) ArrayAltIterator_get,
   (int (*)(void*))   ArrayAltIterator_end,
   (void (*)(void*))  ArrayAltIterator_to_begin,
   (int (*)(void*, void*)) ArrayAltIterator_same,
   (void (*)(void*))  ArrayAltIterator_free
 );
}
//
// void for_each_with_index( ArrayAlt_it(ArrayAlt* array),  void (^callback)(void*, size_t)) {
//   ArrayAltIterator* it = ArrayAltIterator_new(array);
//   size_t count = 0;
//   while(!ArrayAltIterator_end(it)) {
//     callback(ArrayAltIterator_get(it), count++);
//     ArrayAltIterator_next(it);
//   }
//   ArrayAltIterator_free(it);
// }
