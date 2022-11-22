# CLambdaAlgorithms

This project is an exploration on how the blocks extension to C introduced by apple in 2009. The library introduces a number of well known containers and algorithms.

## Containers

All of the following are opaque types (with possibly more than one supported implementation):

- Array (several implementations are given)
- Dictionary (implemented with hash tables, search trees, and rb-trees)
- Graph
- List (implemented with arrays and linked lists)
- Queue
- Set
- Stack
- UnionFindSet

All containers support iterators and a number of utility functions such as `map` and `for_each`.

## Algorithms

- Dijkstra's algorithm
- Editian distance
- Graph Visiting
- HeapSort
- InsertionSort
- Kruskal
- MergeSort
- Prim
- QuickSort
- String utilitiy functions

## Example of usage

```c
#include "array.h"
#include "iterator_functions.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Create a new array with initial capacity of 10
    Array* array = Array_new(10);

    for (int i = 0; i < 10; i++) {
        int* val = malloc(sizeof(int));
        *val = i;
        Array_add(array, val);
    }
    
    for_each(Array_it(array), ^(void* element) {
        printf("%d ", *(int*)element);
    });

    for_each(Array_it(array), ^(void* element) {
        free(element);
    });

    Array_free(array);
    return 0;
}
```