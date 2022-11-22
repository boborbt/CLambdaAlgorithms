#include "multy_way_tree.h"
#include "string_utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "iterator_functions.h"
#include "multywaytree_utils.h"


static MultyWayTree* build_binary_search_tree_from_array(Array* values, size_t start_index, size_t end_index) {
  if(start_index > end_index) {
    return NULL;
  }

  size_t len = end_index - start_index;
  size_t root_pos = start_index + (len / 2);
  char* root_value = Array_at(values, root_pos);

  MultyWayTree* root = MultyWayTree_new(root_value);

  MultyWayTree* left;
  if(root_pos != 0) {
    left = build_binary_search_tree_from_array(values, start_index, root_pos - 1);
  } else {
    left = NULL;
  }
  MultyWayTree* right = build_binary_search_tree_from_array(values, root_pos+1, end_index);

  if(left != NULL) {
    MultyWayTree_add_subtree(root, left);
  } else {
    MultyWayTree_add_child(root, NULL);
  }

  if(right != NULL) {
    MultyWayTree_add_subtree(root, right);
  } else {
    MultyWayTree_add_child(root, NULL);
  }

  return root;
}

static MultyWayTree* build_binary_search_tree(MultyWayTree* tree) {
  Array* values = Array_new(2000);
  for_each(MultyWayTree_it(tree), ^(void* elem) {
    MultyWayTree* node = (MultyWayTree*) elem;
    Array_add(values, MultyWayTree_get(node));
  });

  Array_sort(values, ^(const void* lhs, const void* rhs) {
    return strcmp(lhs, rhs);
  });

  MultyWayTree* result = build_binary_search_tree_from_array(values, 0, Array_size(values)-1);
  Array_free(values);

  return result;
}


int main() {
  FILE* file = fopen("multywaytree_1.csv", "r");
  MultyWayTree* tree = load_tree(file);
  fclose(file);
  print_multy_way_tree_stats("original tree:", tree);

  MultyWayTree* searchTree = build_binary_search_tree(tree);
  print_multy_way_tree_stats("binary search tree:", searchTree);

  FILE* outfile = fopen("binsearchtree.csv", "w");
  save_tree(searchTree, outfile);
  fclose(outfile);

  MultyWayTree_free(searchTree);
  MultyWayTree_free(tree);
  printf("done\n");

  file = fopen("binsearchtree.csv", "r");
  MultyWayTree* st = load_tree(file);
  print_multy_way_tree_stats("reloaded search tree stats:",st);
  MultyWayTree_free(st);

  return 0;
}
