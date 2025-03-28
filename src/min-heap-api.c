/*!
 * \file min-heap.c
 * \date 2025-03-28
 * \authors Antonio Gelain [antonio.gelain2@gmail.com]
 * \authors Dorijan Di Zepp [dorijan.dizepp@eagletrt.it]
 * 
 * \brief Library that implements a minimum heap with an arena allocator
 * \details TODO
 * 
 * \warning TODO
 */

 #include "min-heap.h"
 #include "min-heap-api.h"
 
 #include <string.h>
 #include <stdio.h>
 
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
 
 static inline void min_heap_swap(MinHeapHandler_t *heap, size_t a, size_t b) {
     void *temp = heap->arenaAllocator.items[a].value;
     heap->arenaAllocator.items[a].value = heap->arenaAllocator.items[b].value;
     heap->arenaAllocator.items[b].value = temp;
 }
 
 MinHeapReturnCode min_heap_init(MinHeapHandler_t *heap, size_t max_capacity, int8_t (*compare)(void *, void *)) {
     if (!heap || !compare) return MIN_HEAP_NULL_POINTER;
     heap->capacity = max_capacity;
     heap->compare = compare;
     arena_allocator_api_init(&heap->arenaAllocator);
     return MIN_HEAP_OK;
 }
 
 size_t min_heap_size(MinHeapHandler_t *heap) {
     return heap ? heap->arenaAllocator.size : 0;
 }
 
 bool min_heap_is_empty(MinHeapHandler_t *heap) {
     return min_heap_size(heap) == 0;
 }
 
 bool min_heap_is_full(MinHeapHandler_t *heap) {
     return min_heap_size(heap) >= heap->capacity;
 }
 
 MinHeapReturnCode min_heap_insert(MinHeapHandler_t *heap, void *item) {
    if (!heap || !heap->compare || !item) return MIN_HEAP_NULL_POINTER;
    if (min_heap_is_full(heap)) return MIN_HEAP_FULL;
    
    size_t index = heap->arenaAllocator.size;
    void *data = arena_allocator_api_alloc(&heap->arenaAllocator, sizeof(void *));
    if (!data) return MIN_HEAP_NULL_POINTER;

    // Store the item memory address in the allocated space
    memcpy(data, item, sizeof(void *));
    while (index > 0) {
        size_t parent = MIN_HEAP_PARENT(index);
        if (heap->compare(heap->arenaAllocator.items[parent].value, 
            heap->arenaAllocator.items[index].value) <= 0) break;
        min_heap_swap(heap, parent, index);
        index = parent;
    }
    return MIN_HEAP_OK;
 }
 
 MinHeapReturnCode min_heap_top(MinHeapHandler_t *heap, void *out) {
    if (!heap || !out) return MIN_HEAP_NULL_POINTER;
    if (min_heap_is_empty(heap)) return MIN_HEAP_EMPTY;
    memcpy(out, heap->arenaAllocator.items[0].value, sizeof(void *));
    return MIN_HEAP_OK;
 }
 
 void *min_heap_peek(MinHeapHandler_t *heap) {
     return min_heap_is_empty(heap) ? NULL : heap->arenaAllocator.items[0].value;
 }
 
 MinHeapReturnCode min_heap_remove(MinHeapHandler_t *heap, size_t index, void *out) {
    if (!heap || !heap->compare) return MIN_HEAP_NULL_POINTER;  // Check for null pointers
    if (min_heap_is_empty(heap)) return MIN_HEAP_EMPTY;  // Ensure the heap is not empty
    if (index >= min_heap_size(heap)) return MIN_HEAP_OUT_OF_BOUNDS;  // Validate the index

    // If requested, output the value of the removed item
    if (out) {
        memcpy(out, heap->arenaAllocator.items[index].value, sizeof(void *));
    }

    min_heap_swap(heap, index, heap->arenaAllocator.size - 1);
    //heap->arenaAllocator.items[index].value = heap->arenaAllocator.items[heap->arenaAllocator.size - 1].value;

    --heap->arenaAllocator.size;//as the last element is to be ignored

     // Restore the heap property by "bubbling down" the swapped element
     while (true) {
        size_t left = MIN_HEAP_CHILD_L(index);
        size_t right = MIN_HEAP_CHILD_R(index);
        size_t smallest = index;

        // Check if the left child exists and is smaller than the current node
        if (left < min_heap_size(heap) && heap->compare(heap->arenaAllocator.items[left].value, heap->arenaAllocator.items[smallest].value) < 0)
            smallest = left;

        // Check if the right child exists and is smaller than the current node
        if (right < min_heap_size(heap) && heap->compare(heap->arenaAllocator.items[right].value, heap->arenaAllocator.items[smallest].value) < 0)
            smallest = right;

        if (smallest != index){
            min_heap_swap(heap, index, smallest);
            index = smallest;
        }
        else 
            break;
    }
 
    return MIN_HEAP_OK;  // Return success
 }
 
MinHeapReturnCode min_heap_clear(MinHeapHandler_t *heap) {
    if (!heap) return MIN_HEAP_NULL_POINTER;
    arena_allocator_api_free(&heap->arenaAllocator);
    return MIN_HEAP_OK;
}

signed_size_t min_heap_find(MinHeapHandler_t *heap, void *item) {
    if (!heap || !item || !heap->compare) return -1;
    if (heap->arenaAllocator.size == 0) return -1;
    
    for (size_t index = 0; index < heap->arenaAllocator.size; index++) {
        if (!heap->arenaAllocator.items) {
            return -1;
        }
        if (!heap->arenaAllocator.items[index].value) {
            continue;
        }
        
        void * stored_value = heap->arenaAllocator.items[index].value;
        //printf("Checking index %ld: %d\n", index, stored_value);

        if (heap->compare(stored_value, item) == 0) {
            return index;
        }
    }

    return -1;
}
