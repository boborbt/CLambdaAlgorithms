#include <assert.h>
#include <stdio.h>

#include "iterator.h"
#include "iterator_functions.h"
#include "mem.h"
#include "errors.h"
#include "list.h"
#include "macros.h"


Iterator Iterator_make(
  void* container,
  void*  (*new)(void*),
  void  (*next)(void*),
  void* (*get)(void*),
  int   (*end)(void*),
  int   (*same)(void*, void*),
  void  (*free)(void*)
) {
  Iterator it;

  it.container=container;
  it.new_iterator=new;
  it.next=next;
  it.get=get;
  it.end=end;
  it.same=same;
  it.free=free;

  // bidirectional
  it.move_to = NULL;
  it.size = NULL;

  // random access
  it.prev = NULL;
  it.to_end = NULL;

  // mutable
  it.set = NULL;

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

Iterator MutableIterator_make(
  Iterator iterator,
  void  (*set)(void*, void*)
) {
  iterator.set = set;

  return iterator;
}


static void require_bidirectional_iterator(Iterator it) {
  if(it.to_end==NULL || it.prev==NULL) {
    Error_raise(Error_new(ERROR_ITERATOR_MISUSE, "The given iterator is not a bidirectional iterator as required"));
  }
}

static void require_random_access_iterator(Iterator it) {
  if(it.move_to == NULL || it.size == NULL) {
      Error_raise(Error_new(ERROR_ITERATOR_MISUSE, "The given iterator is not a random access iterator as required"));
  }
}

static void require_mutable_iterator(Iterator it) {
  if(it.set == NULL) {
    Error_raise(Error_new(ERROR_ITERATOR_MISUSE, "The given iterator is not a mutable iterator as required"));
  }
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
  require_bidirectional_iterator(it);

  void* iterator = it.new_iterator(it.container);
  it.to_end(iterator);

  void* result = find_first_(it, iterator, it.prev, condition);

  it.free(iterator);
  return result;
}

size_t binsearch(Iterator it, const void* elem, int (^compare)(const void* lhs, const void* rhs)) {
  require_random_access_iterator(it);

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

void reverse(Iterator it) {
  require_bidirectional_iterator(it);
  require_mutable_iterator(it);

  void* fw_iterator = it.new_iterator(it.container);
  void* bw_iterator = it.new_iterator(it.container);

  it.to_end(bw_iterator);
  int stop = it.same(fw_iterator, bw_iterator);

  while(!stop) {
    void* fw_obj = it.get(fw_iterator);
    void* bw_obj = it.get(bw_iterator);
    it.set(fw_iterator, bw_obj);
    it.set(bw_iterator, fw_obj);

    it.next(fw_iterator);
    stop = it.same(fw_iterator, bw_iterator);

    if(!stop) {
      it.prev(bw_iterator);
    }

    stop = it.same(fw_iterator, bw_iterator);
  }

  it.free(fw_iterator);
  it.free(bw_iterator);
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

static void split(Iterator it, void* iterator, List* list1, List* list2) {
  while(!it.end(iterator)) {
    List_append(list1, it.get(iterator));
    it.next(iterator);

    if(!it.end(iterator)) {
      List_append(list2, it.get(iterator));
      it.next(iterator);
    }
  }
}

static void merge(Iterator it, void* iterator, List* list1, List* list2, int (^compare)(void*, void*)) {
  ListIterator* lit1 = ListIterator_new(list1);
  ListIterator* lit2 = ListIterator_new(list2);

  while(!ListIterator_end(lit1) && !ListIterator_end(lit2)) {
    assert(!it.end(iterator));
    void* obj1 = ListIterator_get(lit1);
    void* obj2 = ListIterator_get(lit2);

    if(compare(obj1, obj2) < 0) {
      it.set(iterator, obj1);
      it.next(iterator);
      ListIterator_next(lit1);
    } else {
      it.set(iterator, obj2);
      it.next(iterator);
      ListIterator_next(lit2);
    }
  }

  while(!ListIterator_end(lit1)) {
    assert(!it.end(iterator));

    void* obj1 = ListIterator_get(lit1);
    it.set(iterator, obj1);
    it.next(iterator);
    ListIterator_next(lit1);
  }

  while(!ListIterator_end(lit2)) {
    assert(!it.end(iterator));

    void* obj2 = ListIterator_get(lit2);
    it.set(iterator, obj2);
    it.next(iterator);
    ListIterator_next(lit2);
  }

  assert(it.end(iterator));
  assert(ListIterator_end(lit1));
  assert(ListIterator_end(lit2));

  ListIterator_free(lit1);
  ListIterator_free(lit2);
}

static int more_than_one_object(Iterator it) {
  int result = 1;
  void* iterator = it.new_iterator(it.container);
  result = !it.end(iterator);

  if(result) {
    it.next(iterator);
    result = !it.end(iterator);
  }

  it.free(iterator);
  return result;
}

void sort(Iterator it, int (^compare)(void*, void*)) {
  require_mutable_iterator(it);
  require_bidirectional_iterator(it);

  if(!more_than_one_object(it)) {
    return;
  }

  void* iterator = it.new_iterator(it.container);

  List* list1 = List_new();
  List* list2 = List_new();

  split(it, iterator, list1, list2);
  it.free(iterator);
  iterator = it.new_iterator(it.container);

  sort(List_it(list1), compare);
  sort(List_it(list2), compare);
  merge(it, iterator, list1, list2, compare);

  List_free(list1, NULL);
  List_free(list2, NULL);

  it.free(iterator);
}


size_t count(Iterator it) {
  __block size_t result = 0;

  for_each(it, ^( UNUSED(void* obj) ){
    result+=1;
  });

  return result;
}
