#include "iterator.h"
#include "iterator_functions.h"
#include "mem.h"

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
  it.new=new;
  it.next=next;
  it.get=get;
  it.end=end;
  it.free=free;

  return it;
}

void for_each(Iterator it, void (^callback)(void*)) {
  void* iterator = it.new(it.container);

  while(!it.end(iterator)) {
    void* elem = it.get(iterator);
    callback(elem);
    it.next(iterator);
  }

  it.free(iterator);
}

void for_each_with_index(Iterator it, void(^callback)(void*, size_t)) {
  void* iterator = it.new(it.container);
  size_t index = 0;

  while(!it.end(iterator)) {
    void* elem = it.get(iterator);
    callback(elem, index++);
    it.next(iterator);
  }

  it.free(iterator);
}


void* find_first(Iterator it, int(^condition)(void* elem)) {
  void* iterator = it.new(it.container);

  while(!it.end(iterator) && condition(it.get(iterator)) == 0) {
    it.next(iterator);
  }

  void* result = it.get(iterator);

  it.free(iterator);
  return result;
}
