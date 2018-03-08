#include "iterator.h"
#include "iterator_functions.h"
#include "mem.h"
#include "errors.h"

#include <stdio.h>

Iterator Iterator_make(
  void* container,
  void*  (*new)(void*),
  void  (*next)(void*),
  void* (*get)(void*),
  int   (*end)(void* it),
  void  (*free)(void*)
) {
  Iterator it;

  it.container=container;
  it.new_iterator=new;
  it.next=next;
  it.get=get;
  it.end=end;
  it.free=free;

  it.move_to = NULL;
  it.size = NULL;

  it.prev = NULL;
  it.to_end = NULL;

  return it;
}

Iterator RandomAccessIterator_make(
  Iterator iterator,
  void   (*move_to)(void*, size_t),
  size_t (*size)(void*)
) {
  iterator.move_to = move_to;
  iterator.size = size;

  return iterator;
}


Iterator BidirectionalIterator_make(
  Iterator iterator,
  void  (*prev)(void*),
  void  (*to_end)(void*)
) {
  iterator.prev = prev;
  iterator.to_end = to_end;

  return iterator;
}

static void for_each_(Iterator it, void* iterator, void (*next)(void*), void (^callback)(void*)) {
  while(!it.end(iterator)) {
    void* elem = it.get(iterator);
    callback(elem);
    next(iterator);
  }
}


void for_each(Iterator it, void (^callback)(void*)) {
  void* iterator = it.new_iterator(it.container);

  for_each_(it, iterator, it.next, callback);

  it.free(iterator);
}

void for_each_reverse(Iterator it, void (^callback)(void*)) {
  void* iterator = it.new_iterator(it.container);
  it.to_end(iterator);

  for_each_(it, iterator, it.prev, callback);

  it.free(iterator);
}

void for_each_with_index(Iterator it, void(^callback)(void*, size_t)) {
  void* iterator = it.new_iterator(it.container);
  size_t index = 0;

  while(!it.end(iterator)) {
    void* elem = it.get(iterator);
    callback(elem, index++);
    it.next(iterator);
  }

  it.free(iterator);
}


static void* find_first_(Iterator it, void* iterator, void (*next)(void*), int(^condition)(void* elem)) {
  while(!it.end(iterator) && condition(it.get(iterator)) == 0) {
    next(iterator);
  }
  void* result = NULL;

  if(!it.end(iterator)) {
    result = it.get(iterator);
  }

  return result;
}


void* find_first(Iterator it, int(^condition)(void* elem)) {
  void* iterator = it.new_iterator(it.container);

  void* result = find_first_(it, iterator, it.next, condition);

  it.free(iterator);
  return result;
}


void* find_last(Iterator it, int(^condition)(void* elem)) {
  if(it.to_end == NULL) {
    Error_raise(Error_new(ERROR_ITERATOR_MISUSE,
      "The given iterator does not implement the BidirectionalIterator APIs as required"));
  }
  void* iterator = it.new_iterator(it.container);
  it.to_end(iterator);
  
  void* result = find_first_(it, iterator, it.prev, condition);

  it.free(iterator);
  return result;
}

size_t binsearch(Iterator it, const void* elem, int (^compare)(const void* lhs, const void* rhs)) {
  if(it.move_to == NULL || it.size == NULL) {
    Error_raise(Error_new(ERROR_ITERATOR_MISUSE,
       "The given iterator does not seem to implement the random access iterator APIs"));
  }

  void* iterator = it.new_iterator(it.container);
  size_t left = 0;
  size_t right = it.size(iterator) - 1;
  size_t current_index = (left + right) / 2;

  it.move_to(iterator, current_index);
  int comparison = compare(elem, it.get(iterator));

  while(left < right && comparison != 0) {
    if(comparison < 0) {
      right = current_index - 1;
    } else {
      left = current_index + 1;
    }

    if(right == (size_t) -1) {
      break;
    }

    current_index = (left + right) / 2;

    it.move_to(iterator, current_index);
    comparison = compare(elem, it.get(iterator));
  }

  it.free(iterator);

  if(comparison == 0) {
    return current_index;
  } else {
    return (size_t) -1;
  }
}



Array* map(Iterator it, void* (^mapping_function)(void*)) {
  Array* result = Array_new(10);
  for_each(it, ^(void* obj) {
    void* elem = mapping_function(obj);
    Array_add(result, elem);
  });

  return result;
}

Array* filter(Iterator it, int (^keep)(void*)) {
  Array* result = Array_new(10);
  for_each(it, ^(void* elem) {
    if(keep(elem)) {
      Array_add(result, elem);
    }
  });

  return result;
}

void* first(Iterator it) {
  void* iterator = it.new_iterator(it.container);
  void* result = NULL;

  if(it.end(iterator)) {
    return NULL;
  }

  result = it.get(iterator);

  it.free(iterator);
  return result;
}

void* last(Iterator it) {
  void* iterator = it.new_iterator(it.container);
  void* result = NULL;

  if(it.to_end != NULL) {
    it.to_end(iterator);
    result = it.get(iterator);
  } else {
    while(!it.end(iterator)) {
      result = it.get(iterator);
      it.next(iterator);
    }
  }

  it.free(iterator);
  return result;
}
