#include "unit_testing.h"
#include "errors.h"

static void test_error_new() {
  Error error = Error_new(ERROR_GENERIC, "error with int:%d and float:%1.2f", 3, 0.5);
  assert_true( ERROR_GENERIC == Error_error_code(error));
  assert_string_equal("error with int:3 and float:0.50", Error_message(error));
}

static void test_error_raise() {
  Error error = Error_new(ERROR_GENERIC, "error with int:%d and float:%f", 3, 0.5);
  assert_exits_with_code(Error_raise(error), ERROR_GENERIC);
}

int main() {
  start_tests("errors");
  test(test_error_new);
  test(test_error_raise);
  end_tests();
  return 0;
}
