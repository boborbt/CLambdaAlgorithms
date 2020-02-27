#include <assert.h>
#include "dictionary.h"
#include "print_time.h"
#include "array.h"
#include <stdio.h>
#include "iterator.h"
#include "iterator_functions.h"

#define NUM_TEST_KEYS 1E7
#define MAX_KEY_VALUE 1E7
#define DATASET_SIZE 6321079


typedef struct {
    int* key;
    int* value;
} IntKeyValue;

static int* Int_new(int v) {
    int* result = (int*) malloc(sizeof(int));
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
    FILE* file = fopen(filename, "r");
    Array* array = Array_new(DATASET_SIZE);
    int line_num = 1;

    while(!feof(file)) {
        int k, v;
        int num_read = fscanf(file, "%d,%d", &k, &v);
        if(num_read == -1 && feof(file)) {
            continue;
        }

        if(num_read != 2) {
            printf("read %d integers instead of 2\n", num_read);
            printf("Error loading the dataset (line %d)\n", line_num);
            exit(1);
        }

        IntKeyValue* kv = (IntKeyValue*) malloc(sizeof(IntKeyValue));
        kv->key = Int_new(k);
        kv->value = Int_new(v);

        Array_add(array, kv);
        line_num++;
    }

    return array;
}

static int* IntKey_new() {
    int* result = (int*) malloc(sizeof(int));
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
}

static void print_usage() {
    printf("Usage: measure_times2 (-a|-d) <filename>\n");
    printf("   -a: use arrays\n");
    printf("   -d: use dictionaries\n");
}


int main(int argc, char const *argv[])
{
    if(argc!=3) {
        print_usage();
        exit(1);
    }

    int use_dictionaries;

    switch(argv[1][1]) {
        case 'd': 
            use_dictionaries = 1;
            break;
        case 'a': 
            use_dictionaries = 0;
            break;
        default:
            printf("Expected -d or -a, but %s found\n", argv[1]);
            print_usage();
            exit(1);
    }
    

    char const* filename = argv[2];
    assert(filename != NULL);

    pt = init_print_time();
    __block Array* kv = NULL;

    PrintTime_print(pt, "Data load...", ^{
        printf("Loading dataset...\n");
        kv = load_dataset(filename);
        printf("Done.\n");
    });

    Array* keys = Array_new(NUM_TEST_KEYS);

    PrintTime_print(pt, "Generating keys...", ^{
        printf("Generating keys...\n");
        for(int i=0; i<NUM_TEST_KEYS; ++i) {
            int* key = IntKey_new();
            Array_add(keys, key);
        }
        printf("Done.\n");
    });

    
    if(use_dictionaries) { 
        test_dictionaries(kv, keys);
    } else {
        test_binsearch(kv, keys);
    }

    return 0;
}
