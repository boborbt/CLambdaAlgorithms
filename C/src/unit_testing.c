#include <stdio.h>
#include <time.h>
#include <math.h>
#include "mem.h"
#include <stdio.h>


#include "unit_testing.h"

static clock_t start_time;
static size_t num_tests;

static void(*focus)() = NULL;

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

void test_focus(void (*test_fun)()) {
  focus = test_fun;
}

// Calls a testing function. The given test function should
// exit the program with an error if the test does not succeed.
void test_ext(void (*test_fun)(), char* test_name, char* file, size_t lineno) {
  if(focus!= NULL && focus!=test_fun) {
      return;
  }

  num_tests += 1;
  printf(".");
  test_fun();

  if( !Mem_all_freed() ) {
    MemStats stats = Mem_stats();
    printf("\nLeaked memory after test: %s at line: %ld of file: %s\n", test_name, lineno, file);
    printf("alloced: %ld\n", stats.alloced_memory);
    printf("freed: %ld\n", stats.freed_memory);
    assert_true(Mem_all_freed());
  }
}
