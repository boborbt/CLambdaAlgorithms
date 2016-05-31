#include "queue.h"


Queue Queue_new() {
  return List_new();
}

void Queue_free(Queue queue) {
  List_free(queue, 0);
}

void Queue_enqueue(Queue queue, void* elem) {
  List_insert(queue, elem);
}

void* Queue_dequeue(Queue queue) {
  ListNode node = List_tail(queue);
  void* elem = ListNode_get(node);
  List_delete_node(queue, node);
  return elem;
}

void* Queue_front(Queue queue) {
  return ListNode_get( List_tail(queue) );
}

size_t Queue_size(Queue queue) {
  return List_length(queue);
}
