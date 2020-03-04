#pragma once

#include "array.h"

Array* Dataset__load(const char* filename, unsigned long int max_lines, void* (^load_fun)(Array* fields));
