#include "print_time.h"
#include <stdio.h>
#include <time.h>

void print_time(void(^fun)()) {
 printf("\n======================\n");
 clock_t start = clock();
 fun();
 clock_t end = clock();

 printf("----------------------\n");
 printf("time:  %10.2lf secs\n", ((double)end-start) / CLOCKS_PER_SEC);
 printf("======================\n\n");
}
