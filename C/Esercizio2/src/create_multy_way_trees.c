#include "multy_way_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "mem.h"

#include "iterator_functions.h"

#include "multywaytree_utils.h"

#define NUM_NODES 1000

typedef struct _NodeNames {
  char** names;
  size_t count;
  size_t current;
}* NodeNames;

static NodeNames NODENAMES = NULL;


static size_t min(size_t a, size_t b) {
  if(a<b) {
    return a;
  } else {
    return b;
  }
}

static size_t random_int2() {
  if(drand48() < 0.5) {
    return 1;
  } else {
    return 2;
  }
}

// We want a random number between 1 and 3 with a probabilty
// distribution highly skewed in favor of 1..2.
static size_t random_int3() {
  if(drand48() < 0.01) {
    return 3;
  }

  return random_int2();
}

static void init_node_names() {
  NODENAMES = (NodeNames) Mem_alloc(sizeof(struct _NodeNames));
  NODENAMES->count = NUM_NODES + 100;
  NODENAMES->current = 0;
  NODENAMES->names = (char**) Mem_alloc(sizeof(char*)*NODENAMES->count);

  char buf[1000];
  for(size_t i=0; i<NODENAMES->count; ++i) {
    sprintf(buf, "N:%04ld", i);
    NODENAMES->names[i] = Mem_strdup(buf);
  }
}

static void reset_node_names() {
  NODENAMES->current = 0;
}

static char* new_node_name() {
  if( NODENAMES == NULL ) {
    init_node_names();
  }

  if( NODENAMES->count <= NODENAMES->current) {
    printf("Asking for nodename number:%ld\n", NODENAMES->current);
  }

  assert( NODENAMES->count > NODENAMES->current );
  return NODENAMES->names[NODENAMES->current++];
}

static void build_multyway_tree(MultyWayTree node, size_t num_nodes, size_t (*random_int)()) {
  if(num_nodes == 0) {
    return;
  }

  size_t num_children = min(num_nodes, random_int());

  for(size_t i=0; i<num_children;++i) {
      char* nodeId = new_node_name();
      assert(nodeId != NULL);
      MultyWayTree_add_child(node, nodeId);
  }

  num_nodes -= num_children;

  Array children = MultyWayTree_children(node);
  size_t children_per_child = num_nodes / num_children;
  __block size_t children_count = 0;

  for_each_with_index( Array_it(children),  ^(void* elem, size_t index) {
    MultyWayTree child = *(MultyWayTree*) elem;

    if(index == num_children - 1) {
      build_multyway_tree(child, num_nodes - children_count, random_int );
    } else {
      build_multyway_tree(child, children_per_child, random_int);
      children_count += children_per_child;
    }
  });

  Array_free(children);
}


int main() {

  MultyWayTree root = MultyWayTree_new(new_node_name());
  build_multyway_tree(root, NUM_NODES, random_int3);

  FILE* file = fopen("multywaytree_1.csv", "w");
  assert(file!=NULL);
  save_tree(root, file);
  fclose(file);

  print_multy_way_tree_stats("multywaytree_1",root);
  MultyWayTree_free(root);

  reset_node_names();
  root = MultyWayTree_new(new_node_name());
  build_multyway_tree(root, NUM_NODES, random_int2);

  file = fopen("multywaytree_2.csv", "w");
  assert(file!=NULL);
  save_tree(root, file);
  fclose(file);

  print_multy_way_tree_stats("multywaytree_2",root);
  MultyWayTree_free(root);

  return 0;
}
