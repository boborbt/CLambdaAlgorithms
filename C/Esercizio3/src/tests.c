#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "dijkstra.h"
#include "priority_queue.h"
#include "unit_testing.h"

PriorityQueue build_pq_fixtures() {
  PriorityQueue pq = PriorityQueue_new();

  PriorityQueue_push(pq, "10", 10.0);
  PriorityQueue_push(pq, "2", 2.0);
  PriorityQueue_push(pq, "4", 4.0);
  PriorityQueue_push(pq, "3", 3.0);
  PriorityQueue_push(pq, "6", 6.0);

  return pq;
}

void test_priority_queue_creation() {
  PriorityQueue pq = PriorityQueue_new();
  assert_equal( 0l, (long) PriorityQueue_size(pq) );
  assert(PriorityQueue_empty(pq));
  PriorityQueue_free(pq);
}

void test_priority_queue_push_min_element() {
  PriorityQueue pq = build_pq_fixtures();
  PriorityQueue_push(pq, "test", 1.0);
  assert_string_equal((char*) PriorityQueue_top_value(pq), "test");
  assert_double_equal(PriorityQueue_top_priority(pq), 1.0);
  PriorityQueue_free(pq);
}

void test_priority_queue_top_value() {
  PriorityQueue pq = build_pq_fixtures();
  assert_string_equal((char*)PriorityQueue_top_value(pq), "2");
  PriorityQueue_free(pq);
}

void test_priority_queue_top_priority() {
  PriorityQueue pq = build_pq_fixtures();
  assert_double_equal(PriorityQueue_top_priority(pq), 2.0);
  PriorityQueue_free(pq);
}

void test_priority_queue_pop() {
  PriorityQueue pq = build_pq_fixtures();
  assert_double_equal(PriorityQueue_top_priority(pq), 2.0);

  assert_equal(5l, (long)PriorityQueue_size(pq));
  PriorityQueue_pop(pq);

  assert_equal(4l, (long) PriorityQueue_size(pq));
  assert(PriorityQueue_top_priority(pq) == 3.0);
}





int main(int argc, char const *argv[]) {
  test_priority_queue_creation();
  test_priority_queue_push_min_element();
  test_priority_queue_top_value();
  test_priority_queue_top_priority();
  test_priority_queue_pop();

  return 0;
}
