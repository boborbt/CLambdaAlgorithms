#include "unit_testing.h"
#include "multy_way_tree.h"


static MultyWayTree* build_fixtures() {
  MultyWayTree* tree = MultyWayTree_new("root");
  MultyWayTree_add_child(tree, "c1");
  MultyWayTree_add_child(tree, "c2");
  MultyWayTree_add_child(tree, "c3");
  MultyWayTree_add_child(tree, "c4");

  Array* children = MultyWayTree_children(tree);
  MultyWayTree* c1 = Array_at(children, 0);

  MultyWayTree_add_child(c1, "c5");
  MultyWayTree_add_child(c1, "c6");

  MultyWayTree* c3 = Array_at(children, 2);
  MultyWayTree_add_child(c3, "c7");

  Array_free(children);

  return tree;
}

static void test_multywaytree_add_child_empty_tree() {
  MultyWayTree* tree = MultyWayTree_new("root");
  MultyWayTree_add_child(tree, "child1");

  Array* children = MultyWayTree_children(tree);
  assert_equal( 1l, Array_size(children));

  MultyWayTree* child = Array_at(children, 0);
  assert_string_equal("child1", (char*)MultyWayTree_get(child));

  Array_free(children);
  MultyWayTree_free(tree);
}

static void test_multywaytree_get() {
  MultyWayTree* tree = MultyWayTree_new("root");
  assert_string_equal("root", MultyWayTree_get(tree));
  MultyWayTree_free(tree);
}

static void test_multywaytree_add_child_middle() {
  MultyWayTree* tree = build_fixtures();

  Array* children = MultyWayTree_children(tree);
  MultyWayTree* c2 = Array_at(children, 1);
  Array_free(children);

  assert_null((void*)MultyWayTree_children(c2));

  MultyWayTree_add_child(c2, "c8");

  children = MultyWayTree_children(tree);
  c2 = Array_at(children, 1);
  Array* c2_children = MultyWayTree_children(c2);
  assert_equal(1l, Array_size(c2_children));
  Array_free(children);

  MultyWayTree* c8 = Array_at(c2_children, 0);
  assert_string_equal("c8", MultyWayTree_get(c8));
  Array_free(c2_children);

  MultyWayTree_free(tree);
}

int main() {
  start_tests("MultyWayTree");
  test(test_multywaytree_add_child_empty_tree);
  test(test_multywaytree_get);
  test(test_multywaytree_add_child_middle);

  end_tests();
  return 0;
}
