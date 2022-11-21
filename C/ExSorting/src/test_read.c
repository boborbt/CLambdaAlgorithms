#include "exsorting_dataset.h"
#include <time.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  if(argc < 2) {
    printf("Usage: test_read <nomefile>\n");
  }

  clock_t start = clock();
  ExSortingDataset_load(argv[1]);
  clock_t end = clock();

  printf("Elapsed: %5.2fs\n", (end-start)/(double)CLOCKS_PER_SEC);
  return 0;
}
