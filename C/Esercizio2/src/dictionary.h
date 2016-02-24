#ifndef __DICTIONARY_H_3NXN3XL378__
#define __DICTIONARY_H_3NXN3XL378__

typedef struct _Dictionary* Dictionary;
typedef int (*KeyComparator)(const void*, const void*);

Dictionary Dictionary_new(KeyComparator comparator);
void Dictionary_free(Dictionary dictionary);

// insert into dictionary the given key/value pair. If key is already
// present, the dictionary is updated with the new value. Otherwise, the
// key/value pair is inserted.
void Dictionary_set(Dictionary dictionary, void* key, void* value);

// Retrieve the value associated with the given key. The found value
// is put into *result.
// If the key is not found, the function returns 0 and leave result untouched.
// Otherwise it returns 1.
int Dictionary_get(Dictionary dictionary, void* key, void** result);


void Dictionary_delete(Dictionary dictionary, void* key);

unsigned int Dictionary_size(Dictionary dictionary);

int Dictionary_efficiency_score();

#endif
