#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "unit_testing.h"

static int compare(const void* left, const void* right) {
  if((long int) left < (long int) right) {
    return -1;
  }

  if((long int) left > (long int) right) {
    return 1;
  }

  return 0;
}

static unsigned int hash(const void* elem) {
  long int k = (long int) elem;
  return (unsigned int)(k*(k+3));
}

static Dictionary build_fixture_tree() {
  KeyInfo keyInfo = KeyInfo_new(compare, hash);
  Dictionary dictionary = Dictionary_new(keyInfo);
  Dictionary_set(dictionary, (void*) 10l, (void*) -10l);
  Dictionary_set(dictionary, (void*)  5l, (void*)  -5l);
  Dictionary_set(dictionary, (void*) 15l, (void*) -15l);
  Dictionary_set(dictionary, (void*)  7l, (void*)  -7l);
  Dictionary_set(dictionary, (void*) 13l, (void*) -13l);
  Dictionary_set(dictionary, (void*) 11l, (void*) -11l);
  Dictionary_set(dictionary, (void*) 18l, (void*) -18l);

  return dictionary;
}


static void test_search_tree_creation() {
  KeyInfo keyInfo = KeyInfo_new(compare, hash);

  Dictionary dictionary = Dictionary_new(keyInfo);
  assert(dictionary != NULL);

  assert_equal((long)Dictionary_size(dictionary), 0l);
  Dictionary_free(dictionary);
}

static void test_search_tree_insert_on_empty_tree() {
  KeyInfo keyInfo = KeyInfo_new(compare, hash);
  Dictionary dictionary = Dictionary_new(keyInfo);
  Dictionary_set(dictionary, (void*) 10l, (void*) -10l);
  assert_equal((long)Dictionary_size(dictionary), 1l);

  long int value = 0;
  Dictionary_get(dictionary, (void*) 10l, (void**)&value);
  assert_equal( value,  -10l );

  Dictionary_free(dictionary);
}

static void test_search_tree_insert_on_full_tree() {
  Dictionary dictionary = build_fixture_tree();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_set(dictionary, (void*) 4l, (void*) -4l);
  assert_equal((long)Dictionary_size(dictionary), 8l);

  long int value = 0;
  Dictionary_get(dictionary, (void*) 4l, (void**)&value);
  assert_equal( value,  -4l );

  Dictionary_free(dictionary);
}

static void test_search_tree_replace_on_full_tree() {
  Dictionary dictionary = build_fixture_tree();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_set(dictionary, (void*) 15l, (void*) -115l);
  assert_equal((long)Dictionary_size(dictionary), 7l);

  long int value = 0;
  Dictionary_get(dictionary, (void*) 15l, (void**)&value);
  assert_equal( value,  -115l );

  Dictionary_free(dictionary);
}

static void test_search_tree_delete_on_empty_tree() {
  KeyInfo keyInfo = KeyInfo_new(compare, hash);
  Dictionary dictionary = Dictionary_new(keyInfo);
  Dictionary_delete(dictionary, (void*) 1l);

  assert(TRUE);
}

static void test_search_tree_delete_root() {
  Dictionary dictionary = build_fixture_tree();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_delete(dictionary, (void*) 10l);
  assert_equal((long)Dictionary_size(dictionary), 6l);

  long int value = 0;
  assert_equal((long) Dictionary_get(dictionary, (void*) 10l, (void**) &value), 0l);

  Dictionary_free(dictionary);
}

static void test_search_tree_delete_mid_node() {
  Dictionary dictionary = build_fixture_tree();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_delete(dictionary, (void*) 15l);
  assert_equal((long)Dictionary_size(dictionary), 6l);

  long int value = 0;
  assert_equal((long) Dictionary_get(dictionary, (void*) 15l, (void**) &value), 0l);

  Dictionary_free(dictionary);
}

static void test_search_tree_delete_leaf() {
  Dictionary dictionary = build_fixture_tree();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_delete(dictionary, (void*) 11l);
  assert_equal((long)Dictionary_size(dictionary), 6l);

  long int value = 0;
  assert_equal((long) Dictionary_get(dictionary, (void*) 11l, (void**) &value), 0l);

  Dictionary_free(dictionary);
}

static void test_search_tree_delete_whole_tree() {
  Dictionary dictionary = build_fixture_tree();
  long int keys[] =  {10l, 5l, 15l, 7l, 13l, 11l, 18l};

  for(int i = 0; i < 7; ++i) {
    Dictionary_delete(dictionary, (void*) keys[i]);
  }

  assert_equal((long)Dictionary_size(dictionary), 0l);
  Dictionary_free(dictionary);
}


static void test_search_tree_get_on_empty_tree() {
  KeyInfo keyInfo = KeyInfo_new(compare, hash);
  Dictionary dictionary = Dictionary_new(keyInfo);

  long int value = 0;
  assert_equal((long)Dictionary_get(dictionary, (void*) 10l, (void**)&value), 0l);

  Dictionary_free(dictionary);
}

static void test_search_tree_get_on_full_tree() {
  Dictionary dictionary = build_fixture_tree();

  long int value = 0;
  assert_equal((long)Dictionary_get(dictionary, (void*) 13l, (void**)&value), 1l);
  assert_equal(-13l, value);

  Dictionary_free(dictionary);
}


static void test_search_tree_get_on_non_present_key() {
  Dictionary dictionary = build_fixture_tree();

  long int value = 0;
  assert_equal((long)Dictionary_get(dictionary, (void*) 21l, (void**)&value), 0l);

  Dictionary_free(dictionary);
}

int main() {
  start_tests("search trees");
  test(test_search_tree_creation);
  test(test_search_tree_insert_on_empty_tree);
  test(test_search_tree_insert_on_full_tree);
  test(test_search_tree_replace_on_full_tree);

  test(test_search_tree_delete_on_empty_tree);
  test(test_search_tree_delete_root);
  test(test_search_tree_delete_mid_node);
  test(test_search_tree_delete_leaf);
  test(test_search_tree_delete_whole_tree);

  test(test_search_tree_get_on_empty_tree);
  test(test_search_tree_get_on_full_tree);
  test(test_search_tree_get_on_non_present_key);
  end_tests();

  return 0;
}
