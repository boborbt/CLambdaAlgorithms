#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0

#define assert_equal(a,b) if((a)!=(b)) {\
   printf("assertion failed at file:%s line:%d. %ld was expected to be equal to %ld\n",\
    __FILE__, __LINE__, (a), (b)); exit(1); }

int compare(const void* left, const void* right) {
  if((long int) left < (long int) right) {
    return -1;
  }

  if((long int) left > (long int) right) {
    return 1;
  }

  return 0;
}

Dictionary build_fixture_tree() {
  Dictionary dictionary = Dictionary_new(compare);
  Dictionary_set(dictionary, (void*) 10l, (void*) -10l);
  Dictionary_set(dictionary, (void*)  5l, (void*)  -5l);
  Dictionary_set(dictionary, (void*) 15l, (void*) -15l);
  Dictionary_set(dictionary, (void*)  7l, (void*)  -7l);
  Dictionary_set(dictionary, (void*) 13l, (void*) -13l);
  Dictionary_set(dictionary, (void*) 11l, (void*) -11l);
  Dictionary_set(dictionary, (void*) 18l, (void*) -18l);

  return dictionary;
}


void test_search_tree_creation() {
  Dictionary dictionary = Dictionary_new(compare);
  assert(dictionary != NULL);

  assert_equal((long)Dictionary_size(dictionary), 0l);
  Dictionary_free(dictionary);
}

void test_search_tree_insert_on_empty_tree() {
  Dictionary dictionary = Dictionary_new(compare);
  Dictionary_set(dictionary, (void*) 10l, (void*) -10l);
  assert_equal((long)Dictionary_size(dictionary), 1l);

  long int value = 0;
  Dictionary_get(dictionary, (void*) 10l, (void**)&value);
  assert_equal( value,  -10l );

  Dictionary_free(dictionary);
}

void test_search_tree_insert_on_full_tree() {
  Dictionary dictionary = build_fixture_tree();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_set(dictionary, (void*) 4l, (void*) -4l);
  assert_equal((long)Dictionary_size(dictionary), 8l);

  long int value = 0;
  Dictionary_get(dictionary, (void*) 4l, (void**)&value);
  assert_equal( value,  -4l );

  Dictionary_free(dictionary);
}

void test_search_tree_replace_on_full_tree() {
  Dictionary dictionary = build_fixture_tree();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_set(dictionary, (void*) 15l, (void*) -115l);
  assert_equal((long)Dictionary_size(dictionary), 7l);

  long int value = 0;
  Dictionary_get(dictionary, (void*) 15l, (void**)&value);
  assert_equal( value,  -115l );

  Dictionary_free(dictionary);
}

void test_search_tree_delete_on_empty_tree() {
  Dictionary dictionary = Dictionary_new(compare);
  Dictionary_delete(dictionary, (void*) 1l);

  assert(TRUE);
}

void test_search_tree_delete_root() {
  Dictionary dictionary = build_fixture_tree();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_delete(dictionary, (void*) 10l);
  assert_equal((long)Dictionary_size(dictionary), 6l);

  long int value = 0;
  assert_equal((long) Dictionary_get(dictionary, (void*) 10l, (void**) &value), 0l);

  Dictionary_free(dictionary);
}

void test_search_tree_delete_mid_node() {
  Dictionary dictionary = build_fixture_tree();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_delete(dictionary, (void*) 15l);
  assert_equal((long)Dictionary_size(dictionary), 6l);

  long int value = 0;
  assert_equal((long) Dictionary_get(dictionary, (void*) 15l, (void**) &value), 0l);

  Dictionary_free(dictionary);
}

void test_search_tree_delete_leaf() {
  Dictionary dictionary = build_fixture_tree();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_delete(dictionary, (void*) 11l);
  assert_equal((long)Dictionary_size(dictionary), 6l);

  long int value = 0;
  assert_equal((long) Dictionary_get(dictionary, (void*) 11l, (void**) &value), 0l);

  Dictionary_free(dictionary);
}

void test_search_tree_get_on_empty_tree() {
  Dictionary dictionary = Dictionary_new(compare);

  long int value = 0;
  assert_equal((long)Dictionary_get(dictionary, (void*) 10l, (void**)&value), 0l);

  Dictionary_free(dictionary);
}

void test_search_tree_get_on_full_tree() {
  Dictionary dictionary = build_fixture_tree();

  long int value = 0;
  assert_equal((long)Dictionary_get(dictionary, (void*) 13l, (void**)&value), 1l);
  assert_equal(-13l, value);

  Dictionary_free(dictionary);
}

void test_search_tree_get_on_non_present_key() {
  Dictionary dictionary = build_fixture_tree();

  long int value = 0;
  assert_equal((long)Dictionary_get(dictionary, (void*) 21l, (void**)&value), 0l);

  Dictionary_free(dictionary);
}



int main(int argc, char const *argv[]) {
  test_search_tree_creation();
  test_search_tree_insert_on_empty_tree();
  test_search_tree_insert_on_full_tree();
  test_search_tree_replace_on_full_tree();

  test_search_tree_delete_on_empty_tree();
  test_search_tree_delete_root();
  test_search_tree_delete_mid_node();
  test_search_tree_delete_leaf();

  test_search_tree_get_on_empty_tree();
  test_search_tree_get_on_full_tree();
  test_search_tree_get_on_non_present_key();

  return 0;
}
