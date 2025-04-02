/*!
 * \file min-heap-basic.c
 * \date 2025-03-28
 * \authors Antonio Gelain [antonio.gelain2@gmail.com]
 * \authors Dorijan Di Zepp [dorijan.dizepp@eagletrt.it]
 *
 * \brief Simple example of a min heap that stores integer values.
 * \details In this example, we allocate both the arena allocator and the heap  
 *      and insert some random numbers into it.  
 *      We then check whether the minimum value is actually at the top of the  
 *      heap, which can be verified by also printing all the values stored in it.  
 *      At the end of the program, all allocated memory is freed to prevent  
 *      memory leaks.
 */
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "min-heap-api.h"

/*!
 * \brief Simple function to compare two integer values.
 */
int8_t min_heap_compare_int(void *a, void *b) {
    int f = *(int *)a;
    int s = *(int *)b;
    if (f < s)
        return -1;
    return f == s ? 0 : 1;
}

int main(void) {
    srand(time(NULL));
    MinHeapHandler_t int_heap;
    ArenaAllocatorHandler_t arena;

    /**
     * Initialize arena allocator for managing 
     * min heap buffer.
     */
    arena_allocator_api_init(&arena);
    min_heap_api_init(&int_heap, sizeof(int), 20, min_heap_compare_int, &arena);

    /**
     * Checking if it is not possible to insert any new 
     * value when the heap is full.
     */
    for (int i = 0; i < 22; ++i) {
        int num = rand() % 100 + 1;
        if (min_heap_api_insert(&int_heap, &num) != MIN_HEAP_OK)
            printf("[ERROR]: Cannot insert element in the heap\n");
    }

    printf("Heap size: %lu\n", min_heap_api_size(&int_heap));
    int min = 0;
    if (min_heap_api_top(&int_heap, &min) == MIN_HEAP_OK)
        printf("Minimum element: %d\n", min);

    printf("Values: ");
    while (!min_heap_api_is_empty(&int_heap)) {
        int num;
        if (min_heap_api_remove(&int_heap, 0, &num) == MIN_HEAP_OK)
            printf("%d ", num);
    }
    printf("\n");

    // Clear the heap and free the arena
    if (min_heap_api_clear(&int_heap) != MIN_HEAP_OK)
        printf("[ERROR]: Cannot clear the heap\n");

    arena_allocator_api_free(&arena);

    return 0;
}