#include "multy_way_tree.h"
#include "multywaytree_utils.h"
#include "string_utils.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "iterator_functions.h"

#define NULL_STR "__NULL__"

void save_tree(MultyWayTree* tree, FILE* file) {
  if(tree == NULL) {
    return;
  }

  char* value = (char*) MultyWayTree_get(tree);
  if(value == NULL) {
    return;
  }

  fprintf(file, "%s", value);

  Array* array = MultyWayTree_children(tree);
  if(array == NULL) {
    fprintf(file, "\n");
    return;
  }

  for_each(Array_it(array), ^(void* elem) {
    MultyWayTree* child = elem;
    char* childValue = (char*) MultyWayTree_get(child);
    if(childValue == NULL) {
      childValue = NULL_STR;
    }
    fprintf(file, ",%s", childValue);
  });

  fprintf(file, "\n");

  for_each(Array_it(array), ^(void* elem) {
    MultyWayTree* child =  elem;
    save_tree(child, file);
  });

  Array_free(array);
}



static int String_compare(const void* s1, const void* s2) {
  if(s1==NULL) {
    return -1;
  }

  if(s2 == NULL) {
    return 1;
  }

  return strcmp((const char*) s1, (const char*) s2);
}


void print_multy_way_tree_stats(char* msg, MultyWayTree* tree) {
  printf("%s:\n", msg);
  printf("\ttree height :%ld\n", MultyWayTree_height(tree));
  printf("\ttree max bf :%ld\n", MultyWayTree_max_branching_factor(tree));
  printf("\ttree size :%ld\n", MultyWayTree_size(tree));
}


MultyWayTree* load_tree(FILE* file) {
  MultyWayTree* tree=NULL;

  char* buf = NULL;
  size_t bufsize = 0;
  ssize_t linelen;
  while( ( linelen = getline(&buf, &bufsize, file)) > 0) {
    buf[linelen-1] = '\0'; // substituting last character with
    Array* pieces = String_split(buf, ',');
    char* nodeId = Array_at(pieces, 0);
    assert(strcmp(nodeId, NULL_STR) != 0);

    MultyWayTree* node;
    if(tree==NULL) {
      tree = MultyWayTree_new(nodeId);
    }

    node = MultyWayTree_find(tree, nodeId, String_compare);
    assert(node != NULL);

    for(size_t i=1; i<Array_size(pieces); ++i) {
      char* childId = Array_at(pieces, i);
      if(strcmp(childId, NULL_STR) == 0) {
        childId = NULL;
      }
      MultyWayTree_add_child(node, childId);
    }

    Array_free(pieces);
  }

  return tree;
}
