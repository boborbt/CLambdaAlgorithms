#ifndef __PRINT_TIME_H_JXCI3MX__
#define __PRINT_TIME_H_JXCI3MX__

#include "dictionary.h"

typedef struct _PrintTime* PrintTime;

PrintTime PrintTime_new(Dictionary header, const char* out_file);
void PrintTime_free(PrintTime);

double PrintTime_print(PrintTime pt, const char* label, void(^fun)());
void PrintTime_save(PrintTime pt);

#endif
