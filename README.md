# MIN-HEAP

This library implements all the functions used to create and manage a [minimum heap](https://it.wikipedia.org/wiki/Heap_(struttura_dati))
data structure. An arena allocator is used to dynamically allocate the array.

> [!IMPORTANT]
> This library does not guarantee that the functions will be executed correctly
> if used with interrupts or multi-threaded architectures

A min heap is a **partially ordered** binary tree-based data structure that guarantees that:
1. The first element is always the minimum of all the elements
2. Each element has a value always lower than the value of the children

> [!TIP]
> A *zero-indexed* array can be represented as a tree by choosing the first element as the root node
> and the element at index $i * 2 + 1$ as the left child and $i * 2 + 2$ as the right child

This library implements:
- Insertion of an item in $O(log N)$ time complexity
- Removal of any item (given the index in the heap) in $O(log N)$ time complexity
- Search of any item in $O(N)$ time complexity

> [!NOTE]
> Removal of an item without the index requires a linear search of the array,
> which implies linear time complexity (i.e. *no bueno*)

## Dependencies

This library uses [ArenaAllocator](https://github.com/eagletrt/libarena-allocator-sw.git) for memory management. Make sure to initialize ArenaAllocatorHandler_t to use data array, as shown in the following section.

## Usage

To create a min heap, first declare a variable using `MinHeapHandler_t`. In order to initialize it, an arena allocator is needed.\
For example:
```c
MinHeapHandler_t int_heap;
MinHeapHandler_t point_heap;
ArenaAllocatorHandler_t arena;

arena_allocator_api_init(&arena);
min_heap_init(&int_heap, sizeof(int), 20, min_heap_compare_int, &arena);
min_heap_init(&point_heap, sizeof(struct), 13, min_heap_compare_point, &arena);
```

To remove any trace of the buffer, the solely `min_heap_clear` function is not sufficient as it doesn't deallocate the data buffer.
To do so, `arena_allocator_api_free` will be used as follows:

```c
min_heap_clear(&int_heap);
min_heap_clear(&point_heap);
arena_allocator_api_free(&arena);
```

The compare function signature is as follows:
```c
int8_t compare(void * a, void * b);
```

The two parameters `a` and `b` are the two items that should be compared and has to
be casted to the type of the item used inside the heap. \
The return value should be:
- Negative(-1): if `a` is *less than* `b`
- Zero(0): if `a` *equals* `b`
- Positive(1): if `a` is *greater than* `b`

For example:
```c
int8_t int_compare(void * a, void * b) {
    int f = *(int *)a;
    int s = *(int *)b;
    if (f < s) return -1;
    return f == s ? 0 : 1;
}
```

The `MinHeapReturnCode` enum is return by most of the functions of this library
and **should always be checked** before attempting other operations with the data structure.

## Examples

For more info check the [examples](./examples/) folder.