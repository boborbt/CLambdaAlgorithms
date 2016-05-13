#include <stdio.h>
#include <time.h>
#include <math.h>

#include "unit_testing.h"

static clock_t start_time;
static size_t num_tests;

void start_tests(const char* msg) {
  start_time = clock();
  num_tests = 0;
  printf("Starting tests on:%s\n", msg);
}

// Ends a testing session
void end_tests() {
  double elapsed_time = (clock() - start_time)/(double)CLOCKS_PER_SEC;
  printf("\n%ld tests passed in %4.5f seconds\n\n", num_tests, elapsed_time);
}

// Calls a testing function. The given test function should
// exit the program with an error if the test does not succeed.
void test(void (*test_fun)()) {
  num_tests += 1;
  printf(".");
  test_fun();
}
