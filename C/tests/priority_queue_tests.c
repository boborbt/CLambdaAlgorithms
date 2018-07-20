#include "unit_testing.h"
#include "priority_queue.h"
#include "double_container.h"
#include "iterator.h"
#include "iterator_functions.h"

static void free_fixtures(PriorityQueue* pq) {
  KeyInfo* key_info = PriorityQueue_key_info(pq);
  if(key_info) {
    KeyInfo_free(key_info);
  }

  PriorityQueue_free(pq);
}

static PriorityQueue* fixtures() {
  KeyInfo* key_info = KeyInfo_new(Key_string_compare, Key_string_hash);
  PriorityQueue* pq = PriorityQueue_new(PQOrder_ascending, key_info);
  assert_true(PriorityQueue_empty(pq));

  PriorityQueue_push(pq, "p1", 1.0);
  PriorityQueue_push(pq, "p2", 2.0);
  PriorityQueue_push(pq, "p3", 10.0);
  PriorityQueue_push(pq, "p4", 5.0);
  PriorityQueue_push(pq, "p5", 1.0);

  assert_equal(PriorityQueue_size(pq), 5l);
  return pq;
}

static PriorityQueue* fixtures_descending() {
  KeyInfo* key_info = KeyInfo_new(Key_string_compare, Key_string_hash);
  PriorityQueue* pq = PriorityQueue_new(PQOrder_descending, key_info);
  assert_true(PriorityQueue_empty(pq));

  PriorityQueue_push(pq, "p1", 1.0);
  PriorityQueue_push(pq, "p2", 2.0);
  PriorityQueue_push(pq, "p3", 10.0);
  PriorityQueue_push(pq, "p4", 5.0);
  PriorityQueue_push(pq, "p5", 1.0);

  assert_equal(PriorityQueue_size(pq), 5l);
  return pq;
}

static void test_priority_queue_push() {
  PriorityQueue* pq = fixtures();
  size_t size = PriorityQueue_size(pq);

  PriorityQueue_push(pq, "test", 4.0);
  assert_equal(size + 1, PriorityQueue_size(pq));

  double priority;
  PriorityQueue_get_priority(pq, "test", &priority);
  assert_double_equal(4.0, priority, 0.001);

  free_fixtures(pq);
}

static void test_priority_queue_pop() {
  PriorityQueue* pq = fixtures();
  char* strings[] = {"p1", "p5", "p2", "p4", "p3"};
  double priorities[] = {1.0, 1.0, 2.0, 5.0, 10.0};

  for(int i=0; i<5; ++i) {
    char* string = (char*) PriorityQueue_top_value(pq);
    assert_string_equal(strings[i], string);
    PriorityQueue_pop(pq);

    for(int j=i+1; j < 5; j++) {
      double priority;
      assert_true(PriorityQueue_get_priority(pq, strings[j], &priority));
      assert_double_equal(priorities[j], priority, 0.0001);
    }
  }

  assert_true(PriorityQueue_empty(pq));

  free_fixtures(pq);
}

static void test_priority_queue_top_value() {
  PriorityQueue* pq = fixtures();

  assert_string_equal("p1", PriorityQueue_top_value(pq));

  PriorityQueue_push(pq, "test", 0.0);
  assert_string_equal("test", PriorityQueue_top_value(pq));

  // top value does not change when adding a lower priority element
  PriorityQueue_push(pq, "test2", 20.0);
  assert_string_equal("test", PriorityQueue_top_value(pq));

  free_fixtures(pq);
}

static void test_priority_queue_top_priority() {
  PriorityQueue* pq = fixtures();

  assert_double_equal(1.0, PriorityQueue_top_priority(pq),0.001);

  PriorityQueue_push(pq, "test", 0.0);
  assert_double_equal(0.0, PriorityQueue_top_priority(pq),0.001);

  // top value does not change when adding a lower priority element
  PriorityQueue_push(pq, "test2", 20.0);
  assert_string_equal("test", PriorityQueue_top_value(pq));

  free_fixtures(pq);
}

static void test_priority_queue_decrease_priority() {
  PriorityQueue* pq = fixtures();

  assert_string_equal("p1", PriorityQueue_top_value(pq));
  PriorityQueue_decrease_priority(pq, "p4", 0.0);
  assert_string_equal("p4", PriorityQueue_top_value(pq));

  free_fixtures(pq);
}

static void test_priority_queue_pop_descending() {
  PriorityQueue* pq = fixtures_descending();
  char* strings[] = {"p3", "p4", "p2", "p1", "p5"};

  for(int i=0; i<5; ++i) {
    char* string = (char*) PriorityQueue_top_value(pq);
    assert_string_equal(strings[i], string);
    PriorityQueue_pop(pq);
  }

  assert_true(PriorityQueue_empty(pq));

  free_fixtures(pq);
}

static void test_priority_queue_large_queue() {
  PriorityQueue* pq = PriorityQueue_new(PQOrder_ascending, NULL);
  unsigned int num_insertions = 1024 * 4;
  for(unsigned int i=0; i <= num_insertions; ++i) {
    DoubleContainer* dbl = DoubleContainer_new((double)i);
    assert_not_null(dbl);
    PriorityQueue_push(pq, dbl, (double) i);
  }

  for(unsigned int i=0; i <= num_insertions; ++i) {
    DoubleContainer* dbl = PriorityQueue_top_value(pq);
    assert_double_equal((double) i, DoubleContainer_get(dbl), 0.0001);
    PriorityQueue_pop(pq);

    DoubleContainer_free(dbl);
  }

  PriorityQueue_free(pq);
}

static void test_priority_queue_iterators() {
  PriorityQueue* pq = fixtures_descending();
  __block double last_priority = 20.0;

  for_each(PriorityQueue_it(pq), ^(void* obj) {
    PQElem* elem = (PQElem*) obj;
    assert_true(last_priority >= elem->priority);
    last_priority = elem->priority;
  });

  free_fixtures(pq);
}


int main() {
  start_tests("Priority Queue");

  test(test_priority_queue_push);
  test(test_priority_queue_pop);
  test(test_priority_queue_top_value);
  test(test_priority_queue_top_priority);
  test(test_priority_queue_decrease_priority);
  test(test_priority_queue_pop_descending);
  test(test_priority_queue_large_queue);
  test(test_priority_queue_iterators);

  end_tests();
  return 0;
}
