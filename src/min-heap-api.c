/*!
 * \file min-heap-api.c
 * \date 2025-03-28
 * \authors Antonio Gelain [antonio.gelain@studenti.unitn.it]
 * \authors Dorijan Di Zepp [dorijan.dizepp@eagletrt.it]
 *
* \brief Library that implements a minimum heap with an arena allocator to
 *      dynamically allocate the buffer
 * 
 * \details A min heap is a binary heap where the smallest element is 
 *      always at the root. It maintains the heap property, meaning each 
 *      parent node has a value less than or equal to its children. 
 *      It is also a complete binary tree, meaning all levels are fully 
 *      filled except possibly the last, which is filled from left to right. 
 *      The main operations include insertion, deletion and top value. 
 * 
 * \warning The data buffer will not be deallocated automatically but has to be freed 
 *      by using the arena allocator.
 */

#include "min-heap-api.h"

#include <string.h>

/*!
* \brief Macros to get the parent and children indices given the current item index
*
* \param I The current item index
* \param B The size of the item in bytes
* \return The parent, left child or right child respectively
*/
#define MIN_HEAP_PARENT(I) ((I - 1) / 2)
#define MIN_HEAP_CHILD_L(I) ((I) * 2 + 1)
#define MIN_HEAP_CHILD_R(I) ((I) * 2 + 2)

static inline void min_heap_swap(MinHeapHandler_t *heap, void *a, void *b) {
    uint8_t aux[heap->data_size];//local buffer to avoid invalid memory access
    memcpy(aux, a, heap->data_size);
    memcpy(a, b, heap->data_size);
    memcpy(b, aux, heap->data_size);
}

MinHeapReturnCode min_heap_init(
    MinHeapHandler_t *heap,
    size_t data_size,
    size_t capacity,
    int8_t (*compare)(void *, void *),
    ArenaAllocatorHandler_t *arena) {
    if (heap == NULL || compare == NULL || arena == NULL)
        return MIN_HEAP_NULL_POINTER;
    heap->data_size = data_size;
    heap->size = 0;
    heap->capacity = capacity;
    heap->compare = compare;
    heap->data = arena_allocator_api_calloc(arena, data_size, capacity);
    if (heap->data == NULL)
        return MIN_HEAP_NULL_POINTER;
    return MIN_HEAP_OK;
}

size_t min_heap_size(MinHeapHandler_t *heap) {
    return heap == NULL ? 0U : heap->size;
}

bool min_heap_is_empty(MinHeapHandler_t *heap) {
    return heap == NULL ? true : heap->size == 0;
}

bool min_heap_is_full(MinHeapHandler_t *heap) {
    return heap == NULL ? true : heap->size >= heap->capacity;
}

MinHeapReturnCode min_heap_top(MinHeapHandler_t *heap, void *out) {
    if (heap == NULL || out == NULL || heap->data == NULL)
        return MIN_HEAP_NULL_POINTER;
    if (heap->size == 0)
        return MIN_HEAP_EMPTY;
    memcpy(out, heap->data, heap->data_size);
    return MIN_HEAP_OK;
}

void *min_heap_peek(MinHeapHandler_t *heap) {
    if (heap == NULL || heap->size == 0)
        return NULL;
    return heap->data;
}

MinHeapReturnCode min_heap_clear(MinHeapHandler_t *heap) {
    if (heap == NULL)
        return MIN_HEAP_NULL_POINTER;
    heap->size = 0;
    return MIN_HEAP_OK;
}

MinHeapReturnCode min_heap_insert(MinHeapHandler_t *heap, void *item) {
    if (heap == NULL || item == NULL || heap->compare == NULL || heap->data == NULL)
        return MIN_HEAP_NULL_POINTER;
    if (heap->size == heap->capacity)
        return MIN_HEAP_FULL;

    // Insert item at the end of the heap
    const size_t data_size = heap->data_size;
    size_t cur = heap->size;
    uint8_t *base = (uint8_t *)heap->data;
    memcpy(base + cur * data_size, item, data_size);
    ++heap->size;

    // Restore heap properties
    size_t parent = MIN_HEAP_PARENT(cur);
    while (cur != 0 && heap->compare(base + cur * data_size, base + parent * data_size) < 0) {
        // Swap items
        min_heap_swap(heap, base + cur * data_size, base + parent * data_size);

        // Update indices
        cur = parent;
        parent = MIN_HEAP_PARENT(cur);
    }
    return MIN_HEAP_OK;
}

MinHeapReturnCode min_heap_remove(MinHeapHandler_t *heap, size_t index, void *out) {
    if (heap == NULL || heap->compare == NULL)
        return MIN_HEAP_NULL_POINTER;
    if (heap->size == 0)
        return MIN_HEAP_EMPTY;
    if (index >= heap->size)
        return MIN_HEAP_OUT_OF_BOUNDS;

    // Swap the error with the last one in the heap (if not the same)
    uint8_t *base = (uint8_t *)heap->data;
    const size_t data_size = heap->data_size;
    if (heap->size > 1)
        min_heap_swap(heap, base + index * data_size, base + (heap->size - 1) * data_size);

    // Remove last element
    --heap->size;

    // Copy element
    if (out != NULL)
        memcpy(out, base + heap->size * data_size, data_size);

    if (index == heap->size)
        return MIN_HEAP_OK;

    // Restore heap properties
    int8_t cmp = heap->compare(base + index * data_size, base + heap->size * data_size);
    // Up-heapify
    if (cmp < 0) {
        size_t parent = MIN_HEAP_PARENT(index);
        while (index != 0 && heap->compare(base + index * data_size, base + parent * data_size) < 0) {
            // Swap items
            min_heap_swap(heap, base + index * data_size, base + parent * data_size);

            // Update indices
            index = parent;
            parent = MIN_HEAP_PARENT(index);
        }
    }
    // Down-heapify
    else if (cmp > 0) {
        // Restore heap properties
        size_t l = MIN_HEAP_CHILD_L(index);
        size_t r = MIN_HEAP_CHILD_R(index);
        size_t child = (r >= heap->size)
                           ? l
                           : (heap->compare(base + l * data_size, base + r * data_size) < 0
                                  ? l
                                  : r);

        // Until a leaf is reached (or the parent has only the left child)
        while (r < heap->size && heap->compare(base + child * data_size, base + index * data_size) < 0) {
            min_heap_swap(heap, base + index * data_size, base + child * data_size);

            // Update indices
            index = child;
            l = MIN_HEAP_CHILD_L(index);
            r = MIN_HEAP_CHILD_R(index);
            child = (r >= heap->size)
                        ? l
                        : (heap->compare(base + l * data_size, base + r * data_size) < 0
                               ? l
                               : r);
        }

        // Check left child
        if (l < heap->size && heap->compare(base + l * data_size, base + index * data_size) < 0)
            min_heap_swap(heap, base + index * data_size, base + child * data_size);
    }
    return MIN_HEAP_OK;
}

signed_size_t min_heap_find(MinHeapHandler_t *heap, void *item) {
    if (heap == NULL || item == NULL || heap->compare == NULL || heap->size == 0 || heap->data == NULL)
        return -1;

    for (size_t i = 0; i < heap->size; ++i) {
        if (heap->compare(item, (uint8_t *)heap->data + heap->data_size * i) == 0)
            return i;
    }

    return -1;
}