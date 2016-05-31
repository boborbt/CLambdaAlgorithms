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

static size_t hash(const void* elem) {
  long int k = (long int) elem;
  return (size_t)(k*(k+3));
}

static Dictionary build_fixture_dictionary() {
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


static void test_dictionary_creation() {
  KeyInfo keyInfo = KeyInfo_new(compare, hash);

  Dictionary dictionary = Dictionary_new(keyInfo);
  assert(dictionary != NULL);

  assert_equal((long)Dictionary_size(dictionary), 0l);
  Dictionary_free(dictionary);
}

static void test_dictionary_insert_on_empty_dictionary() {
  KeyInfo keyInfo = KeyInfo_new(compare, hash);
  Dictionary dictionary = Dictionary_new(keyInfo);
  Dictionary_set(dictionary, (void*) 10l, (void*) -10l);
  assert_equal((long)Dictionary_size(dictionary), 1l);

  long int value = 0;
  Dictionary_get(dictionary, (void*) 10l, (void**)&value);
  assert_equal( value,  -10l );

  Dictionary_free(dictionary);
}

static void test_dictionary_insert_on_full_dictionary() {
  Dictionary dictionary = build_fixture_dictionary();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_set(dictionary, (void*) 4l, (void*) -4l);
  assert_equal((long)Dictionary_size(dictionary), 8l);

  long int value = 0;
  Dictionary_get(dictionary, (void*) 4l, (void**)&value);
  assert_equal( value,  -4l );

  Dictionary_free(dictionary);
}

static void test_dictionary_replace_on_full_dictionary() {
  Dictionary dictionary = build_fixture_dictionary();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_set(dictionary, (void*) 15l, (void*) -115l);
  assert_equal((long)Dictionary_size(dictionary), 7l);

  long int value = 0;
  Dictionary_get(dictionary, (void*) 15l, (void**)&value);
  assert_equal( value,  -115l );

  Dictionary_free(dictionary);
}

static void test_dictionary_delete_on_empty_dictionary() {
  KeyInfo keyInfo = KeyInfo_new(compare, hash);
  Dictionary dictionary = Dictionary_new(keyInfo);
  Dictionary_delete(dictionary, (void*) 1l);

  assert(TRUE);
}

static void test_dictionary_delete_root() {
  Dictionary dictionary = build_fixture_dictionary();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_delete(dictionary, (void*) 10l);
  assert_equal((long)Dictionary_size(dictionary), 6l);

  long int value = 0;
  assert_equal((long) Dictionary_get(dictionary, (void*) 10l, (void**) &value), 0l);

  Dictionary_free(dictionary);
}

static void test_dictionary_delete_mid_node() {
  Dictionary dictionary = build_fixture_dictionary();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_delete(dictionary, (void*) 15l);
  assert_equal((long)Dictionary_size(dictionary), 6l);

  long int value = 0;
  assert_equal((long) Dictionary_get(dictionary, (void*) 15l, (void**) &value), 0l);

  Dictionary_free(dictionary);
}

static void test_dictionary_delete_leaf() {
  Dictionary dictionary = build_fixture_dictionary();
  assert_equal( (long) Dictionary_size(dictionary), 7l);

  Dictionary_delete(dictionary, (void*) 11l);
  assert_equal((long)Dictionary_size(dictionary), 6l);

  long int value = 0;
  assert_equal((long) Dictionary_get(dictionary, (void*) 11l, (void**) &value), 0l);

  Dictionary_free(dictionary);
}

static void test_dictionary_delete_whole_dictionary() {
  Dictionary dictionary = build_fixture_dictionary();
  long int keys[] =  {10l, 5l, 15l, 7l, 13l, 11l, 18l};

  for(int i = 0; i < 7; ++i) {
    Dictionary_delete(dictionary, (void*) keys[i]);
  }

  assert_equal((long)Dictionary_size(dictionary), 0l);
  Dictionary_free(dictionary);
}

static void test_dictionary_delete_element_twice() {
  Dictionary dictionary = build_fixture_dictionary();
  Dictionary_delete(dictionary, (void*)15l);
  assert_equal(6l, Dictionary_size(dictionary));

  Dictionary_delete(dictionary, (void*)15l);
  assert_equal(6l, Dictionary_size(dictionary));
  assert_false(Dictionary_get(dictionary, (void*)15l, NULL));
}


static void test_dictionary_get_on_empty_dictionary() {
  KeyInfo keyInfo = KeyInfo_new(compare, hash);
  Dictionary dictionary = Dictionary_new(keyInfo);

  long int value = 0;
  assert_equal((long)Dictionary_get(dictionary, (void*) 10l, (void**)&value), 0l);

  Dictionary_free(dictionary);
}

static void test_dictionary_get_on_full_dictionary() {
  Dictionary dictionary = build_fixture_dictionary();

  long int value = 0;
  assert_equal((long)Dictionary_get(dictionary, (void*) 13l, (void**)&value), 1l);
  assert_equal(-13l, value);

  Dictionary_free(dictionary);
}


static void test_dictionary_get_on_non_present_key() {
  Dictionary dictionary = build_fixture_dictionary();

  long int value = 0;
  assert_equal((long)Dictionary_get(dictionary, (void*) 21l, (void**)&value), 0l);

  Dictionary_free(dictionary);
}

static void test_dictionary_iterator_on_empty_dictionary() {
  KeyInfo keyInfo = KeyInfo_new(compare, hash);
  Dictionary dictionary = Dictionary_new(keyInfo);
  DictionaryIterator it = DictionaryIterator_new(dictionary);
  assert_true( DictionaryIterator_end(it) )
}

static int find_key_value(KeyValue** elems, size_t size, void* key, void* value) {
  for(size_t i=0; i<size; ++i) {
    if(elems[i]->key == key && elems[i]->value == value) {
      return 1;
    }
  }

  return 0;
}

static void test_dictionary_foreach_dictionary_key_value() {
  Dictionary dictionary = build_fixture_dictionary();
  __block KeyValue** found_elems = (KeyValue**) malloc(sizeof(KeyValue*)*7);
  __block size_t count = 0;

  foreach_dictionary_key_value(dictionary, ^(KeyValue* kv) {
    assert_not_equal(7l, count);
    found_elems[count++] = kv;
  });

  assert_equal(7l, count);

  assert_true(find_key_value(found_elems, 7, (void*) 10l, (void*) -10l));
  assert_true(find_key_value(found_elems, 7, (void*)  5l, (void*)  -5l));
  assert_true(find_key_value(found_elems, 7, (void*) 15l, (void*) -15l));
  assert_true(find_key_value(found_elems, 7, (void*)  7l, (void*)  -7l));
  assert_true(find_key_value(found_elems, 7, (void*) 13l, (void*) -13l));
  assert_true(find_key_value(found_elems, 7, (void*) 11l, (void*) -11l));
  assert_true(find_key_value(found_elems, 7, (void*) 18l, (void*) -18l));

  free(found_elems);
  Dictionary_free(dictionary);
}

int main() {
  start_tests("search dictionarys");
  test(test_dictionary_creation);
  test(test_dictionary_insert_on_empty_dictionary);
  test(test_dictionary_insert_on_full_dictionary);
  test(test_dictionary_replace_on_full_dictionary);

  test(test_dictionary_delete_on_empty_dictionary);
  test(test_dictionary_delete_root);
  test(test_dictionary_delete_mid_node);
  test(test_dictionary_delete_leaf);
  test(test_dictionary_delete_whole_dictionary);
  test(test_dictionary_delete_element_twice);

  test(test_dictionary_get_on_empty_dictionary);
  test(test_dictionary_get_on_full_dictionary);
  test(test_dictionary_get_on_non_present_key);

  test(test_dictionary_iterator_on_empty_dictionary);

  test(test_dictionary_foreach_dictionary_key_value);
  end_tests();

  return 0;
}
