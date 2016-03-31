#include "dataset.h"
#include <time.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  clock_t start = clock();
  Dataset* dataset;
  dataset = Dataset_load(argv[1]);
  clock_t end = clock();

  printf("Elapsed: %5.2fs\n", (end-start)/(double)CLOCKS_PER_SEC);
  return 0;
}
