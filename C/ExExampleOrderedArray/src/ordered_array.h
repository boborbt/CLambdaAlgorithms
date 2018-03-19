#pragma once

// --------------------------------------------------------------------------------
// OrderedArray
//
// An ordered array is an ordered container based on arrays. All operations mutating
// the array will preserve the order induced by the given compare function.
// --------------------------------------------------------------------------------

typedef struct _OrderedArray OrderedArray;

// CompareCallback is the type of functions callbacks tha can be used with ordered arrays.
// A compare function needs to accept two objects o1 and o2 and return:
// -1 if o1 is strictly smaller than o2
// 0 if o1 is equal to o2
// +1 if o1 is strictly larger than o2

typedef int (*CompareCallback)(void*, void*);

// OrderedArray constructor
//
// Parameters:
//  - compare: a comparison function. the data structure will keep the objects sorted according
//    to the ordering induced by this function.
// Returns: a newly built ordered array.
// Memory: the memory alloced by this function needs to be disposed by the user using the
//    OrderedArray_free function. The memory occupied by the objects contained in the array
//    is not managed by the data structure (the user is responsible for allocing and deallocing it).
OrderedArray* OrderedArray_new(CompareCallback compare);

// Ordered array destructor
// Frees the memory taken by the container data structure. It does not free the memory taken by
// the contained elements.
//
// Parameters:
//  - ordered_array: the array to be freed
// Memory: see comment above
void OrderedArray_free(OrderedArray* ordered_array);

// Returns the number of objects currently stored in the array.
//
// Parameters:
//  - ordered_array: the array to be queried
long OrderedArray_count(OrderedArray* ordered_array);

// Adds a new object to the array
//
// Parameters:
//  - ordered_array: the array where the object has to be stored
//  - obj: the object to be inserted
// Memory:
//  The method will automatically increment the memory associated with the array when needed.
void OrderedArray_add(OrderedArray* ordered_array, void* obj);

// Removes an object from the array
//
// Parameters:
//  - ordered_array: the array containing the object to be removed
//  - index: position inside the array (starting from 0) of the object to be removed
// Memory:
//   The memory of the removed object will not be freed by this function (i.e., it should be
//   freed by the user).
void OrderedArray_remove_at(OrderedArray* ordered_array, long index);

// Returns the object at the given index
//
// Parameters:
//  - ordered_array: the array to be queried
//  - index: the index inside the array (starting from 0) of the desired object 
void* OrderedArray_at(OrderedArray* ordered_array, long index);
