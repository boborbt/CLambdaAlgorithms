#include "unit_testing.h"
#include "queue.h"


static void test_enqueue_and_dequeue() {
  Queue* q = Queue_new();
  assert_equal(0l, Queue_size(q));

  Queue_enqueue(q, "a");
  assert_equal(1l, Queue_size(q));

  Queue_enqueue(q, "b");
  assert_equal(2l, Queue_size(q));

  assert_string_equal("a", Queue_dequeue(q));
  assert_equal(1l, Queue_size(q));

  assert_string_equal("b", Queue_dequeue(q));
  assert_equal(0l, Queue_size(q));

  Queue_free(q);
}

int main() {
  start_tests("Queue");
  test(test_enqueue_and_dequeue);
  end_tests();
  return 0;
}
