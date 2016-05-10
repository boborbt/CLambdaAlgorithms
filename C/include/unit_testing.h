#pragma once

#include <stdio.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define DOUBLE_MIN_DIST


#define assert_not_null(a) if((a)==NULL) {\
  printf("assertion failed at file:%s line:%d. %p was expected to be not null\n",\
    __FILE__, __LINE__, (void*)(a)); exit(1); }

#define assert_null(a) if((a)!=NULL) {\
  printf("assertion failed at file:%s line:%d. %p was expected to be null\n",\
    __FILE__, __LINE__, (a)); exit(1); }


#define assert_equal(a,b) if((a)!=(b)) {\
   printf("assertion failed at file:%s line:%d. %ld was expected to be equal to %ld\n",\
    __FILE__, __LINE__, (a), (b)); exit(1); }

#define assert_equal32(a,b) if((a)!=(b)) {\
   printf("assertion failed at file:%s line:%d. %d was expected to be equal to %d\n",\
    __FILE__, __LINE__, (a), (b)); exit(1); }

#define assert_double_equal(a,b,min_dist) if(fabs((a)-(b))>(min_dist)) {\
   printf("assertion failed at file:%s line:%d. %f was expected to be equal to %f\n",\
    __FILE__, __LINE__, (a), (b)); exit(1); }


#define assert_string_equal(a,b) if(strcmp((a),(b))) {\
   printf("assertion failed at file:%s line:%d. %s was expected to be equal to %s\n",\
    __FILE__, __LINE__, (a), (b)); exit(1); }

#define assert_true(a) if(!(a)) {\
   printf("assertion failed at file:%s line:%d. %d was expected to be true\n",\
    __FILE__, __LINE__, (a)); exit(1); }


// Starts a testing session
void start_tests(const char* msg);

// Ends a testing session
void end_tests(void);

// Calls a testing function. The given test function should
// exit the program with an error if the test does not succeed.
void test(void (*test_fun)());
