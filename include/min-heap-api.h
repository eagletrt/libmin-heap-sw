/*!
 * \file min-heap-api.h
 * \date 2025-03-28
 * \authors Antonio Gelain [antonio.gelain@studenti.unitn.it]
 * \authors Dorijan Di Zepp [dorijan.dizepp@eagletrt.it]
 *
 * \brief Library that implements a minimum heap with a static array without
 *      dinamic memory allocation
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

 #ifndef MIN_HEAP_API_H
 #define MIN_HEAP_API_H
 
 #include "min-heap.h"
 #include "arena-allocator-api.h"
 
 /*!
   * \brief Initialize the minimum heap structure
   *
   * \details If the min_heap_new macro is used this function is not needed
   *
   * \param heap The min heap structur handler
   * \param type The type of the items
   * \param capacity The maximum number of the items in the heap
   * \param cmp_callback A pointer to a function that should compare two items of the heap
   * \param arena The arena allocator handler needed to allocate the data buffer
   * \return MinHeapReturnCode
   *     - MIN_HEAP_NULL_POINTER if the heap handler, the callback or the arena are NULL
   *     - MIN_HEAP_OK otherwise
   */
 #define min_heap_init(heap, type, capacity, cmp_callback, arena) \
     _min_heap_init(                                              \
         (MinHeapInterface *)(heap),                              \
         sizeof(type),                                            \
         capacity,                                                \
         cmp_callback,                                            \
         (ArenaAllocatorHandler_t *)(arena))
 
 /*!
   * \brief Get the number of elements inside the heap
   *
   * \param heap The heap handler structure
   * \return size_t The current size
   */
 #define min_heap_size(heap) _min_heap_size((MinHeapInterface *)(heap))
 
 /*!
   * \brief Check if the heap is empty
   * \details If heap is NULL it is considered as empty
   *
   * \param heap The heap handler structure
   * \return bool True if the heap is empty, false otherwise
   */
 #define min_heap_is_empty(heap) _min_heap_is_empty((MinHeapInterface *)(heap))
 
 /*!
   * \brief Check if the heap is full
   * \details If heap is NULL it is considered as full
   *
   * \param heap The heap handler structure
   * \return bool True if the heap is full, false otherwise
   */
 #define min_heap_is_full(heap) _min_heap_is_full((MinHeapInterface *)(heap))
 
 /*!
   * \brief Get a copy of the first element in the heap (the minimum)
   * \attention The out variable has to be an address of a variable compatible
   * with the items in the heap (e.g. int if the heap contains integers)
   * 
   * \param heap The heap handler structure
   * \param out The address of the variable where the copy is stored
   * \return MinHeapReturnCode
   *     - MIN_HEAP_NULL_POINTER if the heap handler or out are NULL
   *     - MIN_HEAP_EMPTY if the heap is empty
   *     - MIN_HEAP_OK otherwise
   */
 #define min_heap_top(heap, out) _min_heap_top((MinHeapInterface *)(heap), (void *)(out))
 
 /*!
   * \brief Get a reference to the first element in the heap (the minimum)
   * \attention The return value can be NULL
   * \attention Keep in mind that the content of the first item in the heap
   * can change even if the pointer don't
   *
   * \param heap The heap handler structure
   * \return void * A pointer to the minimum element
   */
 #define min_heap_peek(heap) _min_heap_peek((MinHeapInterface *)(heap))
 
 /*!
   * \brief Clear the heap removing all elements
   *
   * \param heap The heap handler structure
   * \return MinHeapReturnCode
   *     - MIN_HEAP_NULL_POINTER if the heap handler is NULL
   *     - MIN_HEAP_OK otherwise
   */
 #define min_heap_clear(heap) _min_heap_clear((MinHeapInterface *)(heap))
 
 /*!
   * \brief Insert an element in the heap
   *
   * \param heap The heap handler structure
   * \param item The item to insert
   * \return MinHeapReturnCode
   *     - MIN_HEAP_NULL_POINTER if the heap handler, the compare callback or the item are NULL
   *     - MIN_HEAP_FULL if the heap is full
   *     - MIN_HEAP_OK otherwise
   */
 #define min_heap_insert(heap, item) _min_heap_insert((MinHeapInterface *)(heap), (void *)(item))
 
 /*!
   * \brief Remove an element from the heap
   * \attention 'out' can be NULL
   * \details If 'out' is not NULL the item data is copied into it
   *
   * \param heap The heap handler structure
   * \param index The index of the item to remove from the heap
   * \param out The removed item (has to be an address)
   * \return MinHeapReturnCode
   *     - MIN_HEAP_NULL_POINTER if the heap handler or the compare callback are NULL
   *     - MIN_HEAP_EMPTY if the heap is empty
   *     - MIN_HEAP_OUT_OF_BOUNDS if the index is greater than the size of the heap
   *     - MIN_HEAP_OK otherwise
   */
 #define min_heap_remove(heap, index, out) _min_heap_remove((MinHeapInterface *)(heap), index, (void *)(out))
 
 /*!
   * \brief Find the index of an item in the heap array
   * \details This function has linear time complexity, use it wisely
   *
   * \param heap The heap handler structure
   * \param item The item to find
   * \return signed_size_t The index in the heap of the item if found, -1 otherwise
   */
 #define min_heap_find(heap, item) _min_heap_find((MinHeapInterface *)(heap), (void *)(item))
 
 /******************************************/
 /*   DO NOT USE THE FOLLOWING FUNCTIONS   */
 /*         USE THE MACRO INSTEAD          */
 /******************************************/
 
 MinHeapReturnCode _min_heap_init(
     MinHeapInterface *heap,
     size_t data_size,
     size_t capacity,
     int8_t (*compare)(void *, void *),
     ArenaAllocatorHandler_t *arena);
 size_t _min_heap_size(MinHeapInterface *heap);
 bool _min_heap_is_empty(MinHeapInterface *heap);
 bool _min_heap_is_full(MinHeapInterface *heap);
 MinHeapReturnCode _min_heap_top(MinHeapInterface *heap, void *out);
 void *_min_heap_peek(MinHeapInterface *heap);
 MinHeapReturnCode _min_heap_clear(MinHeapInterface *heap);
 MinHeapReturnCode _min_heap_insert(MinHeapInterface *heap, void *item);
 MinHeapReturnCode _min_heap_remove(MinHeapInterface *heap, size_t index, void *out);
 signed_size_t _min_heap_find(MinHeapInterface *heap, void *item);
 
 #endif