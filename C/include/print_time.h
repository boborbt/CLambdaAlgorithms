#ifndef __PRINT_TIME_H_JXCI3MX__
#define __PRINT_TIME_H_JXCI3MX__

// Takes a block, execute it and print the cpu time it took to complete
// Returns the cpu time in secs taken by fun.
double print_time(void(^fun)());

#endif
