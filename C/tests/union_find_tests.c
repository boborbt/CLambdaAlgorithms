#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "union_find.h"
#include "unit_testing.h"

static void test_union() {
  UnionFindSet s1 = UnionFindSet_new("x1");
  UnionFindSet s2 = UnionFindSet_new("x2");

  assert_false(UnionFindSet_same(s1, s2));
  UnionFindSet_union(s1,s2);
  assert_true(UnionFindSet_same(s1, s2));

  UnionFindSet_free(s1);
  UnionFindSet_free(s2);
}

static void test_find() {
  UnionFindSet s1 = UnionFindSet_new("x1");
  UnionFindSet s2 = UnionFindSet_new("x2");
  UnionFindSet s3 = UnionFindSet_new("x3");

  UnionFindSet_union(s1,s2);

  assert_true(UnionFindSet_find(s1) == s1 || UnionFindSet_find(s1) == s2);
  assert_true(UnionFindSet_find(s2) == s1 || UnionFindSet_find(s2) == s2);
  assert_false(UnionFindSet_find(s3) == s1 || UnionFindSet_find(s3) == s2);
  assert_true(UnionFindSet_find(s3) == s3);

  UnionFindSet_free(s1);
  UnionFindSet_free(s2);
  UnionFindSet_free(s3);
}

static void test_same() {
  UnionFindSet s1 = UnionFindSet_new("x1");
  UnionFindSet s2 = UnionFindSet_new("x2");
  UnionFindSet s3 = UnionFindSet_new("x3");

  UnionFindSet_union(s1,s2);

  assert_true(UnionFindSet_same(s1,s2));
  assert_true(UnionFindSet_same(s2,s1));
  assert_false(UnionFindSet_same(s3,s1));
  assert_false(UnionFindSet_same(s3,s2));
  assert_true(UnionFindSet_same(s3,s3));

  UnionFindSet_free(s1);
  UnionFindSet_free(s2);
  UnionFindSet_free(s3);
}



int main() {
  start_tests("Union find");
  test(test_union);
  test(test_find);
  test(test_same);

  end_tests();

  return 0;
}
