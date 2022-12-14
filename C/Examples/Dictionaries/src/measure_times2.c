#include <assert.h>
#include "dictionary.h"
#include "print_time.h"
#include "array.h"
#include <stdio.h>
#include "iterator.h"
#include "iterator_functions.h"
#include "mem.h"

#define NUM_TEST_KEYS 1E7
#define MAX_KEY_VALUE 1E7
#define DATASET_SIZE 6321079


typedef struct {
    int* key;
    int* value;
} IntKeyValue;

static int* Int_new(int v) {
    int* result = (int*) Mem_alloc(sizeof(int));
    *result = v;
    return result;
}


static PrintTime* init_print_time() {
    PrintTime* pt = PrintTime_new(NULL);

    PrintTime_add_header(pt,"esercizio", "3");

    return pt;
}

static PrintTime* pt;

static Array* load_dataset(char const* filename) {
  __block Array* array; 
  
  PrintTime_print(pt, "Data load...", ^{
    printf("Loading dataset...\n");

    FILE* file = fopen(filename, "r");
    array = Array_new(DATASET_SIZE);
    int line_num = 1;

    while (!feof(file)) {
      int k, v;
      int num_read = fscanf(file, "%d,%d", &k, &v);
      if (num_read == -1 && feof(file)) {
        continue;
      }

      if (num_read != 2) {
        printf("read %d integers instead of 2\n", num_read);
        printf("Error loading the dataset (line %d)\n", line_num);
        exit(1);
      }

      IntKeyValue* kv = (IntKeyValue*)Mem_alloc(sizeof(IntKeyValue));
      kv->key = Int_new(k);
      kv->value = Int_new(v);

      Array_add(array, kv);
      line_num++;
    }

    printf("Done.\n");
  });

  return array;
}

static int* IntKey_new() {
    int* result = (int*) Mem_alloc(sizeof(int));
    int random  = (int)(drand48() * MAX_KEY_VALUE );

    *result = random;
    return result;
}

static void test_dictionaries(Array* kv, Array* keys) {
    Dictionary* dictionary = Dictionary_new(KeyInfo_new(Key_int_compare, Key_int_hash));

    PrintTime_print(pt, "Populating dictionary...", ^{
        printf("Populating the dictionary\n");
        for_each(Array_it(kv), ^(void* value) {
            IntKeyValue* elem = (IntKeyValue*) value;
            Dictionary_set(dictionary, elem->key, elem->value);
        });
    });


    PrintTime_print(pt, "Accessing dictionary...", ^{
        printf("Accessing the dictionary\n");
        __block int num_found = 0;
        __block int num_tested = 0;
        for_each(Array_it(keys), ^(void* value) {
            int* key = (int*) value;
            int* result;

            if(Dictionary_get(dictionary, key, (void**)&result)) {
                num_found += 1;
            }
            num_tested += 1;
        });
        printf("num tested:%d num found: %d\n", num_tested, num_found);
    });

    PrintTime_print(pt, "Freeing dictionary...", ^{ 
        Dictionary_free(dictionary);
    });
}

static void test_binsearch(Array* kv, Array* keys) {
    Array* array = Array_new(DATASET_SIZE);
    int (^kv_compare)(const void* el1, const void* el2) = ^(const void* el1, const void* el2) {
            const IntKeyValue* kv1 = (const IntKeyValue*) el1;
            const IntKeyValue* kv2 = (const IntKeyValue*) el2;

            return Key_int_compare(kv1->key, kv2->key);
    };

    PrintTime_print(pt, "Populating the array...", ^{
        printf("Populating the array\n");
        for_each(Array_it(kv), ^(void* value) {
            IntKeyValue* elem = (IntKeyValue*) value;
            Array_add(array, elem);
        });

        Array_sort(array, kv_compare);
    });

    PrintTime_print(pt, "Accessing array...", ^{
        printf("Accessing the array\n");
        __block int num_found = 0;
        __block int num_tested = 0;
        for_each(Array_it(keys), ^(void* value) {
            IntKeyValue key;
            key.key = (int*) value;
            size_t key_index = binsearch(Array_it(array), &key, kv_compare);

            if(key_index != (size_t)-1) {
                num_found += 1;
            }
            num_tested += 1;
        });
        printf("num tested:%d num found: %d\n", num_tested, num_found);
    });

    PrintTime_print(pt, "Freeing array...", ^{
        Array_free(array);
    });
}

static void print_usage() {
    printf("Usage: measure_times2 (-a|-d) <filename>\n");
    printf("   -a: use arrays\n");
    printf("   -d: use dictionaries\n");
}


static Array* generate_keys(){
  Array* keys = Array_new(NUM_TEST_KEYS);

  PrintTime_print(pt, "Generating keys...", ^{
    printf("Generating keys...\n");
    for (int i = 0; i < NUM_TEST_KEYS; ++i) {
      int* key = IntKey_new();
      Array_add(keys, key);
    }
    printf("Done.\n");
  });

  return keys;
}

static void cleanup(Array* kv, Array* keys) {
  PrintTime_print(pt, "Cleaning up...", ^{
    printf("Freeing %lu objects\n", Array_size(kv)*3 + 1 + Array_size(keys) + 1);
    for_each(Array_it(kv), ^(void* obj) {
      IntKeyValue* tbd = (IntKeyValue*)obj;
      Mem_free(tbd->key);
      Mem_free(tbd->value);
      Mem_free(tbd);
    });

    Array_free(kv);

    for_each(Array_it(keys), ^(void* obj) {
      int* key = (int*)obj;
      Mem_free(key);
    });

    Array_free(keys);
  });
}

static int parse_args(int argc, char const* argv[]) {
  if (argc != 3) {
    print_usage();
    exit(1);
  }

  switch (argv[1][1]) {
    case 'd':
      return 1;
    case 'a':
      return 0;
    default:
      printf("Expected -d or -a, but %s found\n", argv[1]);
      print_usage();
      exit(1);
  }
}


int main(int argc, char const *argv[])
{
    int use_dictionaries = parse_args(argc, argv);
    char const* filename = argv[2];
    assert(filename != NULL);

    pt = init_print_time();
    Array* kv = load_dataset(filename);
    Array* keys = generate_keys();

    
    if(use_dictionaries) { 
        test_dictionaries(kv, keys);
    } else {
        test_binsearch(kv, keys);
    }

    cleanup(kv, keys);

    PrintTime_free(pt);

    return 0;
}
