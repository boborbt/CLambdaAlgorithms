#pragma once

#include <stdio.h>
#include "multy_way_tree.h"

void save_tree(MultyWayTree tree, FILE* file);
void print_multy_way_tree_stats(char* msg, MultyWayTree tree);
MultyWayTree load_tree(FILE* file);
