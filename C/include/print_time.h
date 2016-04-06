#pragma once

#include "dictionary.h"

typedef struct _PrintTime* PrintTime;

PrintTime PrintTime_new(Dictionary header, const char* out_file);
void PrintTime_free(PrintTime);

double PrintTime_print(PrintTime pt, char* label, void(^fun)());
void PrintTime_save(PrintTime pt);
