/*!
 * \file min-heap-api.h
 * \date 2026-03-18
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

#ifndef MIN_HEAP_API_H
#define MIN_HEAP_API_H

#include "min-heap.h"
#include "arena-allocator-api.h"

/*!
 * \brief Initialize the minimum heap structure
 *
 * \param[in] heap The min heap structure handler
 * \param[in] type The type of the items
 * \param[in] capacity The maximum number of the items in the heap
 * \param[in] cmp_callback A pointer to a function that should compare two items of the heap
 * \param[in] arena The arena allocator handler needed to allocate the data buffer
 * \retval MIN_HEAP_RC_NULL_POINTER if the heap handler, the callback or the arena are \c NULL
 * \retval MIN_HEAP_RC_OK otherwise
 */
enum MinHeapReturnCode min_heap_api_init(
    struct MinHeapHandler *const heap,
    const size_t data_size,
    const size_t capacity,
    int8_t (*compare)(void *, void *),
    struct ArenaAllocatorHandler *const arena);

/*!
 * \brief Get the number of elements inside the heap
 *
 * \param[in] heap The heap handler structure
 * \return size_t The current size
 */
size_t min_heap_api_size(const struct MinHeapHandler *const heap);

/*!
 * \brief Check if the heap is empty
 * \details If heap is \c NULL it is considered as empty
 *
 * \param[in] heap The heap handler structure
 * \return \c true if the heap is empty, \c false otherwise
 */
bool min_heap_api_is_empty(const struct MinHeapHandler *const heap);

/*!
 * \brief Check if the heap is full
 * \details If heap is \c NULL it is considered as full
 *
 * \param[in] heap The heap handler structure
 * \return \c true if the heap is full, \c false otherwise
 */
bool min_heap_api_is_full(const struct MinHeapHandler *const heap);

/*!
 * \brief Get a copy of the first element in the heap (the minimum)
 * \attention The out variable has to be an address of a variable compatible
 * with the items in the heap (e.g. int if the heap contains integers)
 * 
 * \param[in] heap The heap handler structure
 * \param[out] out The address of the variable where the copy is stored
 * \retval MIN_HEAP_RC_NULL_POINTER if the heap handler or \c out are \c NULL
 * \retval MIN_HEAP_RC_EMPTY if the heap is empty
 * \retval MIN_HEAP_RC_OK otherwise
 */
enum MinHeapReturnCode min_heap_api_top(const struct MinHeapHandler *const heap, void *const out);

/*!
 * \brief Get a reference to the first element in the heap (the minimum)
 * \attention The return value can be \c NULL
 * \attention Keep in mind that the content of the first item in the heap
 * can change even if the pointer doesn't
 *
 * \param[in] heap The heap handler structure
 * \return A pointer to the minimum element
 */
void *min_heap_api_peek(const struct MinHeapHandler *const heap);

/*!
 * \brief Clear the heap removing all elements
 *
 * \param heap The heap handler structure
 * \retval MIN_HEAP_RC_NULL_POINTER if the heap handler is \c NULL
 * \retval MIN_HEAP_RC_OK otherwise
 */
enum MinHeapReturnCode min_heap_api_clear(struct MinHeapHandler *const heap);

/*!
 * \brief Insert an element in the heap
 *
 * \param[in] heap The heap handler structure
 * \param[in] item The item to insert
 * \retval MIN_HEAP_RC_NULL_POINTER if the heap handler, the compare callback or the item are \c NULL
 * \retval MIN_HEAP_RC_FULL if the heap is full
 * \retval MIN_HEAP_RC_OK otherwise
 */
enum MinHeapReturnCode min_heap_api_insert(struct MinHeapHandler *const heap, const void *const item);

/*!
 * \brief Remove an element from the heap
 * \attention \c out can be \c NULL
 * \details If \c out is **not** \c NULL the item data is copied into it
 *
 * \param[in] heap The heap handler structure
 * \param[in] index The index of the item to remove from the heap
 * \param[out] out The removed item (has to be an address)
 * \retval MIN_HEAP_RC_NULL_POINTER if the heap handler or the compare callback are \c NULL
 * \retval MIN_HEAP_RC_EMPTY if the heap is empty
 * \retval MIN_HEAP_RC_OUT_OF_BOUNDS if the index is greater than the size of the heap
 * \retval MIN_HEAP_RC_OK otherwise
 */
enum MinHeapReturnCode min_heap_api_remove(struct MinHeapHandler *const heap, size_t index, void *const out);

/*!
 * \brief Find the index of an item in the heap array
 * \details This function has linear time complexity, use it wisely
 *
 * \param[in] heap The heap handler structure
 * \param[in] item The item to find
 * \return long The index in the heap of the item if found, -1 otherwise
 */
long min_heap_api_find(const struct MinHeapHandler *const heap, void *const item);

#endif