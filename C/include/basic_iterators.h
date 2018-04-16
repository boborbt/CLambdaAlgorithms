#pragma once

#include <stdio.h>
#include "iterator.h"

// BASIC ITERATORS
//
// basic iterators are iterators over a number of basic types. They are useful
// to allow using the iterator functions (for_each, sort, reverse, ...) onto objects
// that are not dealt with the provided containers. For intance:
//
//  -  Number_it: can be used to iterate a block a fixed number of times, e.g.
//    ```
//      for_each(Number_it(100), ^(void* obj){
//        printf("%ld\n", NUM(obj));
//      });
//    ```
//  - TextFile_it: can be used to read the content of a file, e.g.:
//    ```
//      for_each(TextFile_it("myfile.txt", '\n'), ^(void* obj){
//        printf("%s\n", (char*) buf);
//      });
//    ```
//
//   - Char_it: can be used to iterate over a string, e.g.:
//    ```
//      reverse(Char_it(mystring));
//    ```



// This is a convenience macro to dereference the objects returned by
// iterations started using Number_it
// Example:
//    for_each(Number_it(100), ^(void* obj) {
//      printf("number: %ld\n", NUM(obj));
//    });
#define NUM(a) (*(unsigned long*) (a))

// Creates a number iterator. The iterator will iterate from 0 to the given integer.
Iterator Number_it(unsigned long);

// Creates a file iterator. The iterator will iterate over all lines of the file (one lines
// per iteration step) up to the end of the file.
Iterator TextFile_it(const char* filename, char delimiter);


#define CH(a) (*(char*) (a))

// Creates a character iterator over the given string. The resulting iterator will be
// a bidirectional, mutable iterator.
Iterator Char_it(char* string);
