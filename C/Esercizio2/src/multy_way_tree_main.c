#include "multy_way_tree.h"
#include "string_utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "multywaytree_utils.h"



static int String_ref_compare(const void* v1, const void* v2) {
  const char* s1 = *( char* const*) v1;
  const char* s2 = *( char* const*) v2;

  return strcmp(s1, s2);
}

static MultyWayTree build_binary_search_tree_from_array(Array values, size_t start_index, size_t end_index) {
  if(start_index > end_index) {
    return NULL;
  }

  size_t len = end_index - start_index;
  size_t root_pos = start_index + (len / 2);
  char* root_value = *(char**) Array_at(values, root_pos);

  MultyWayTree root = MultyWayTree_new(root_value);

  MultyWayTree left;
  if(root_pos != 0) {
    left = build_binary_search_tree_from_array(values, start_index, root_pos - 1);
  } else {
    left = NULL;
  }
  MultyWayTree right = build_binary_search_tree_from_array(values, root_pos+1, end_index);

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

static MultyWayTree build_binary_search_tree(MultyWayTree tree) {
  Array values = Array_new(2000, sizeof(MultyWayTree));
  for_each(MultyWayTree_it(tree), ^(void* elem) {
    Array_add(values, elem);
  });

  Array_sort(values, String_ref_compare);

  return build_binary_search_tree_from_array(values, 0, Array_size(values)-1);
}


int main() {
  FILE* file = fopen("multywaytree_2.csv", "r");
  MultyWayTree tree = load_tree(file);
  fclose(file);

  MultyWayTree searchTree = build_binary_search_tree(tree);
  print_multy_way_tree_stats("binary search tree:", searchTree);

  FILE* outfile = fopen("binsearchtree.csv", "w");
  save_tree(searchTree, outfile);
  fclose(outfile);

  MultyWayTree_free(tree);
  printf("done\n");

  file = fopen("binsearchtree.csv", "r");
  MultyWayTree st = load_tree(file);
  print_multy_way_tree_stats("reloaded search tree stats:",st);
  MultyWayTree_free(st);

  return 0;
}
