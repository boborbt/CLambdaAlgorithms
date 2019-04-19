#include "array.h"
#include <string.h>
#include <stdio.h>
#include "array_g.h"
#include "quick_sort.h"
#include "errors.h"
#include "mem.h"
#include "limits.h"
#include "macros.h"

struct _Array {
  void** carray;
  size_t capacity;
  size_t size;
};

struct _ArrayIterator {
  Array* array;
  size_t current_index;
};


// Constructors
Array* Array_new(size_t capacity) {
  Array* array = (Array*) Mem_alloc(sizeof(struct _Array));
  array->carray = (void*) Mem_alloc(sizeof(void*) * capacity);
  array->size = 0;
  array->capacity = capacity;
  return array;
}


Array* Array_new_by_copying_carray(void* src, size_t size) {
  size_t capacity = size + 100;
  Array* array = (Array*) Mem_alloc(sizeof(struct _Array));
  array->carray = Mem_alloc(sizeof(void*) * capacity);
  array->capacity = capacity;
  array->size = size;

  memcpy(array->carray, src, sizeof(void*) * size );
  return array;
}

Array* Array_dup(Array* array) {
  return Array_new_by_copying_carray(Array_carray(array), Array_size(array));
}

// Destructor
void Array_free(Array* array) {
  if(array) {
    if(array->carray) {
      Mem_free(array->carray);
    }

    Mem_free(array);
  }
}

// Accessors

int Array_same(Array* a1, Array* a2) {
  return a1->size == a2->size && memcmp(a1->carray, a2->carray, sizeof(void*) * a1->size);
}

void* Array_at(Array* array, size_t index) {
  if(index >= Array_size(array)) {
    Error_raise(Error_new(ERROR_INDEX_OUT_OF_BOUND, "Index %ld is out of bounds (0,%ld)", index, Array_size(array) ));
  }

  return array->carray[index];
}

void* Array_carray(Array* array) {
  return array->carray;
}

size_t Array_size(Array* array) {
  return array->size;
}

int Array_empty(Array* array) {
  return Array_size(array) == 0;
}

size_t Array_capacity(Array* array) {
  return array->capacity;
}


// Setters

static void Array_realloc(Array* array) {
  array->capacity *= 2;
  array->carray = Mem_realloc(array->carray, array->capacity * sizeof(void*));
}

void Array_set_size(Array* array, size_t new_size) {
  while(array->capacity < new_size) {
    Array_realloc(array);
  }

  array->size = new_size;
}

void* Array_set(Array* array, size_t index, void* elem) {
  if(index >= array->size) {
    Error_raise(Error_new(ERROR_INDEX_OUT_OF_BOUND,"Array* index (%ld) out of bound in array of size (%ld)\n", index, array->size));
  }

  array->carray[index] = elem;
  return elem;
}

void Array_add(Array* array, void* elem) {
  if(array->size >= array->capacity) {
    Array_realloc(array);
  }

  Array_set(array, array->size++, elem);
}

void Array_insert(Array* array, size_t index, void* elem) {
  if(array->size >= array->capacity) {
    Array_realloc(array);
  }

  memmove(array->carray + index + 1,
          array->carray + index,
          (array->size - index)*sizeof(void*));

  array->size++;
  Array_set(array, index, elem);
}

void Array_remove(Array* array, size_t index) {
  memmove(&array->carray[index],
          &array->carray[index+1],
          (array->size - (index+1)) * sizeof(void*));


  array->size -= 1;
}

void Array_sort(Array* array, int (^compare)(const void*, const void*)) {
  quick_sort_wb(Array_carray(array), Array_size(array), compare);
}

// Iterator
ArrayIterator* ArrayIterator_new(Array* array) {
  ArrayIterator* iterator = (ArrayIterator*) Mem_alloc(sizeof(struct _ArrayIterator*));
  iterator->array = array;
  iterator->current_index = 0;

  return iterator;
}

void ArrayIterator_free(ArrayIterator* iterator) {
  Mem_free(iterator);
}

size_t ArrayIterator_size(ArrayIterator* it) {
  return Array_size(it->array);
}

// Move the iterator to the next element. Do nothing if it is already past the
// end of the container.
void ArrayIterator_next(ArrayIterator* it) {
  if(!ArrayIterator_end(it)) {
    it->current_index += 1;
  }
}

void ArrayIterator_prev(ArrayIterator* it) {
  if(!ArrayIterator_end(it)) {
    it->current_index -= 1;
  }
}

void ArrayIterator_move_to(ArrayIterator* it, size_t new_position) {
  it->current_index = new_position;
}

// Returns 1 if the iterator is past the end of the container (i.e., if
// ArrayIterator_get would return a sensible result), 0 otherwise.
int ArrayIterator_end(ArrayIterator* it) {
  return it->current_index >= it->array->size || it->current_index == (size_t) -1;
}

void ArrayIterator_to_begin(ArrayIterator* it) {
  it->current_index = 0;
}

void ArrayIterator_to_end(ArrayIterator* it) {
  it->current_index = Array_size(it->array) - 1;
}

// Returns the element currently pointed by the iterator
void* ArrayIterator_get(ArrayIterator* it) {
  return Array_at(it->array, it->current_index);
}

// Returns the element currently pointed by the iterator
void ArrayIterator_set(ArrayIterator* it, void* value) {
  Array_set(it->array, it->current_index, value);
}

int ArrayIterator_same(ArrayIterator* it1, ArrayIterator* it2) {
  return it1->array == it2->array && it1->current_index == it2->current_index;
}

static void* ArrayIterator_alloc_obj(ArrayIterator* UNUSED(it)) {
  return NULL;
}

static void* ArrayIterator_copy_obj(ArrayIterator* it, void* UNUSED(to_mem)) {
  return ArrayIterator_get(it);
}

static void ArrayIterator_free_obj(void* UNUSED(obj)) {
  return;
}

Iterator Array_it(Array* array)
{
 Iterator iterator =  Iterator_make(
   array,
   (void* (*)(void*)) ArrayIterator_new,
   (void (*)(void*))  ArrayIterator_next,
   (void* (*)(void*)) ArrayIterator_get,
   (int (*)(void*))   ArrayIterator_end,
   (void (*)(void*)) ArrayIterator_to_begin,
   (int (*)(void*, void*)) ArrayIterator_same,
   (void (*)(void*))  ArrayIterator_free
 );

 iterator = BidirectionalIterator_make(
   iterator,
   (void (*)(void*)) ArrayIterator_prev,
   (void (*)(void*)) ArrayIterator_to_end
 );

 iterator = RandomAccessIterator_make(
   iterator,
   (void (*)(void*, size_t)) ArrayIterator_move_to,
   (size_t (*)(void*)) ArrayIterator_size
 );

 iterator = MutableIterator_make(
   iterator,
   (void (*)(void*, void*)) ArrayIterator_set
 );

 iterator = CloningIterator_make(
   iterator,
   (void* (*)(void*)) ArrayIterator_alloc_obj,
   (void* (*)(void*, void*)) ArrayIterator_copy_obj,
   (void  (*)(void*)) ArrayIterator_free_obj
 );

 return iterator;
}
