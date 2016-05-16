#pragma once

#include "dictionary.h"

typedef struct _PrintTime* PrintTime;

PrintTime PrintTime_new(const char* out_file);
void PrintTime_free(PrintTime);

void PrintTime_add_header(PrintTime pt, const char* key, const char* value);

double PrintTime_print(PrintTime pt, char* label, void(^fun)());
void PrintTime_save(PrintTime pt);
