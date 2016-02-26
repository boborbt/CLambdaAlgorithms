#ifndef __UNIT_TESTING_H_MN2UDBK22__
#define __UNIT_TESTING_H_MN2UDBK22__

#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define assert_equal(a,b) if((a)!=(b)) {\
   printf("assertion failed at file:%s line:%d. %ld was expected to be equal to %ld\n",\
    __FILE__, __LINE__, (a), (b)); exit(1); }

#define assert_double_equal(a,b) if((a)!=(b)) {\
   printf("assertion failed at file:%s line:%d. %f was expected to be equal to %f\n",\
    __FILE__, __LINE__, (a), (b)); exit(1); }


#define assert_string_equal(a,b) if(strcmp((a),(b))) {\
   printf("assertion failed at file:%s line:%d. %s was expected to be equal to %s\n",\
    __FILE__, __LINE__, (a), (b)); exit(1); }


#endif
