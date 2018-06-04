#include <assert.h>
#include <stdio.h>
#include <Block.h>

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

  // cloning
  it.alloc_obj = NULL;
  it.copy_obj = NULL;
  it.free_obj = NULL;

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

Iterator CloningIterator_make(
  Iterator iterator,
  void*  (*alloc_obj)(void*),
  void*  (*copy_obj)(void*, void*),
  void   (*free_obj)(void*)
) {
  iterator.alloc_obj = alloc_obj;
  iterator.copy_obj = copy_obj;
  iterator.free_obj = free_obj;

  return iterator;
}

int is_bidirectional_iterator(Iterator it) {
  return !(it.to_end==NULL || it.prev==NULL);
}

int is_random_access_iterator(Iterator it) {
  return !(it.move_to == NULL || it.size == NULL);
}
int is_mutable_iterator(Iterator it) {
  return !(it.set == NULL);
}
int is_cloning_iterator(Iterator it) {
  return !(it.alloc_obj == NULL || it.copy_obj == NULL || it.free_obj == NULL);
}


void require_bidirectional_iterator(Iterator it) {
  if(!is_bidirectional_iterator(it)) {
    Error_raise(Error_new(ERROR_ITERATOR_MISUSE, "The given iterator is not a bidirectional iterator as required"));
  }
}

void require_random_access_iterator(Iterator it) {
  if(!is_random_access_iterator(it)) {
      Error_raise(Error_new(ERROR_ITERATOR_MISUSE, "The given iterator is not a random access iterator as required"));
  }
}

void require_mutable_iterator(Iterator it) {
  if(!is_mutable_iterator(it)) {
    Error_raise(Error_new(ERROR_ITERATOR_MISUSE, "The given iterator is not a mutable iterator as required"));
  }
}

void require_cloning_iterator(Iterator it) {
  if(!is_cloning_iterator(it)) {
    Error_raise(Error_new(ERROR_ITERATOR_MISUSE, "The given iterator is not a cloning iterator as required"));
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

static void for_each_with_iterator(Iterator it, void(^callback)(void*)) {
  void* iterator = it.new_iterator(it.container);

  while(!it.end(iterator)) {
    callback(iterator);
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
  void* tmp = it.alloc_obj(fw_iterator);

  while(!stop) {
    void* bw_obj = it.get(bw_iterator);

    tmp = it.copy_obj(fw_iterator, tmp);
    it.set(fw_iterator, bw_obj);
    it.set(bw_iterator, tmp);

    it.next(fw_iterator);
    stop = it.same(fw_iterator, bw_iterator);

    if(!stop) {
      it.prev(bw_iterator);
    }

    stop = it.same(fw_iterator, bw_iterator);
  }

  it.free_obj(tmp);
  it.free(fw_iterator);
  it.free(bw_iterator);
}

void replace(Iterator it, void* (^callback)(void*)) {
  require_mutable_iterator(it);
  void* iterator = it.new_iterator(it.container);

  while(!it.end(iterator)) {
    void* elem = it.get(iterator);
    void* obj = callback(elem);

    it.set(iterator, obj);
    it.next(iterator);
  }

  it.free(iterator);
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


// Quicksort implementation

static void swap__(Iterator it, void* iterator_instance, void* tmp_mem[2],  size_t left, size_t right) {
  it.move_to(iterator_instance, left);
  tmp_mem[0] = it.copy_obj(iterator_instance, tmp_mem[0]);
  it.move_to(iterator_instance, right);
  tmp_mem[1] = it.copy_obj(iterator_instance, tmp_mem[1]);

  it.set(iterator_instance, tmp_mem[0]);

  it.move_to(iterator_instance, left);
  it.set(iterator_instance, tmp_mem[1]);
}

static void array_block_swap_to_end__(Iterator it, void* iterator_instance, void* tmp_mem[2], size_t start, size_t end, size_t len) {
  size_t b1_start = start;
  size_t b2_start = (size_t) (end - len + 1);
  for(size_t i=0; i<len; ++i) {
    swap__(it, iterator_instance, tmp_mem, b1_start+i, b2_start+i);
  }
}

static size_t umin__(size_t e1, size_t e2) {
  return e1 < e2 ? e1 : e2;
}

static void partition_3_way__(Iterator it, void* iterator_instance, void* tmp_mem[2], size_t start, size_t end, size_t pivot_pos, size_t* p1, size_t* p2,  int (^compare)(const void*, const void*)) {
  swap__(it, iterator_instance, tmp_mem, end, pivot_pos);
  it.move_to(iterator_instance, end);

  void* pivot = it.alloc_obj(iterator_instance);
  pivot = it.copy_obj(iterator_instance, pivot);

  size_t i = (size_t)(start-1);
  size_t p = end;
  for(size_t j=start; j<p;) {
    it.move_to(iterator_instance, j);
    if(compare(it.get(iterator_instance), pivot)==0) {
      p-=1;
      swap__(it, iterator_instance, tmp_mem, j, p);
      continue;
    }

    it.move_to(iterator_instance, j);
    if(compare(it.get(iterator_instance), pivot) < 0) {
      ++i;
      swap__(it, iterator_instance, tmp_mem, i, j);
    }

    ++j;
  }

  size_t l = umin__(p-1-i, end-p+1);
  array_block_swap_to_end__(it, iterator_instance, tmp_mem, i+1, end, l);

  if(i==(size_t)-1) {
    *p1 = 0;
  } else {
    *p1 = i;
  }

  if(l == p-1-i) {
    *p2 = end-l+1;
  } else {
    *p2 = i+l+1;
  }

  it.free_obj(pivot);
}

static size_t random_int__(size_t size) {
  return (size_t) (drand48() * size);
}

static void sort__qsort_3_way(Iterator it, void* iterator_instance, void* tmp_mem[2], size_t start, size_t end, int (^compare)(const void*, const void*)) {
  if(end <= start ) {
    return;
  }

  size_t pivot_pos = start + random_int__(end-start);
  size_t p1, p2;
  partition_3_way__(it, iterator_instance, tmp_mem, start, end, pivot_pos, &p1, &p2, compare);

  sort__qsort_3_way(it, iterator_instance, tmp_mem, start, p1, compare);
  sort__qsort_3_way(it, iterator_instance, tmp_mem, p2, end, compare);
}

static void sort__default(Iterator it, int (^compare)(const void*, const void*)) {
  require_mutable_iterator(it);
  require_cloning_iterator(it);
  require_random_access_iterator(it);

  void* iterator = it.new_iterator(it.container);
  size_t count = it.size(iterator);
  void* tmp_mem[2];
  tmp_mem[0] = it.alloc_obj(iterator);
  tmp_mem[1] = it.alloc_obj(iterator);

  sort__qsort_3_way(it, iterator, tmp_mem, 0, count-1, compare);

  it.free_obj(tmp_mem[0]);
  it.free_obj(tmp_mem[1]);
  it.free(iterator);
}

// end qsort implementation

static void sort__no_random_access(Iterator it, int (^compare)(const void*, const void*)) {
  require_mutable_iterator(it);
  require_cloning_iterator(it);

  void* iterator = it.new_iterator(it.container);

  Array* array = Array_new(1000);
  for_each_with_iterator(it, ^(void* cloning_iterator) {
    void* new_obj = it.alloc_obj(cloning_iterator);
    new_obj = it.copy_obj(cloning_iterator, new_obj);
    Array_add(array, new_obj);
  });

  Array_sort(array, compare);

  for_each(Array_it(array), ^(void* obj) {
    assert(!it.end(iterator));

    it.set(iterator, obj);
    it.next(iterator);
  });

  for_each(Array_it(array), ^(void* obj) {
    it.free_obj(obj);
  });

  it.free(iterator);
  Array_free(array);
}


size_t count(Iterator it) {
  if(it.size != NULL) {
    void* iterator = it.new_iterator(it.container);
    size_t result = it.size(iterator);
    it.free(iterator);
    return result;
  }
  __block size_t result = 0;

  for_each(it, ^( UNUSED(void* obj) ){
    result+=1;
  });

  return result;
}

void sort(Iterator it, int (^compare)(const void*, const void*)) {
  if(is_random_access_iterator(it)) {
    sort__default(it, compare);
  } else {
    sort__no_random_access(it, compare);
  }
}
