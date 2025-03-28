/*!
 * \file min-heap-api.h
 * \date 2025-03-28
 * \authors Antonio Gelain [antonio.gelain2@gmail.com]
 * \authors Dorijan Di Zepp [dorijan.dizepp@eagletrt.it]
 * 
 * \brief Library that implements the api of a minimum heap
 * \details TODO
 * 
 * \warning TODO
 */

 #ifndef MIN_HEAP_API_H
 #define MIN_HEAP_API_H
 
 #include "min-heap.h"
 #include "arena-allocator-api.h"
 #include <stdint.h>
 #include <stddef.h>
 #include <stdbool.h>
 
 typedef long signed_size_t;
 
 /*!
  * \brief Initialize the minimum heap structure
  *
  * \details If the min_heap_new macro is used this function is not needed
  *
  * \param heap The min heap structur handler
  * \param type The type of the items
  * \param capacity The maximum number of the items in the heap
  * \param cmp_callback A pointer to a function that should compare two items of the heap
  * \return MinHeapReturnCode
  *     - MIN_HEAP_NULL_POINTER if the heap handler or the callback are NULL
  *     - MIN_HEAP_OK otherwise
  */
 MinHeapReturnCode min_heap_init(
     MinHeapHandler_t * heap,
     size_t max_capacity,
     int8_t (* compare)(void *, void *)
 );
     
 /*!
  * \brief Get the number of elements inside the heap
  *
  * \param heap The heap handler structure
  * \return size_t The current size
  */
 size_t min_heap_size(MinHeapHandler_t * heap);
 
 /*!
  * \brief Check if the heap is empty
  * \details If heap is NULL it is considered as empty
  *
  * \param heap The heap handler structure
  * \return bool True if the heap is empty, false otherwise
  */
 bool min_heap_is_empty(MinHeapHandler_t * heap);
 
 /*!
  * \brief Check if the heap is full
  * \details If heap is NULL it is considered as full
  *
  * \param heap The heap handler structure
  * \return bool True if the heap is full, false otherwise
  */
 bool min_heap_is_full(MinHeapHandler_t * heap);
 
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
 MinHeapReturnCode min_heap_top(MinHeapHandler_t * heap, void * out);
 
 /*!
  * \brief Get a reference to the first element in the heap (the minimum)
  * \attention The return value can be NULL
  * \attention Keep in mind that the content of the first item in the heap
  * can change even if the pointer don't
  *
  * \param heap The heap handler structure
  * \return void * A pointer to the minimum element
  */
 void * min_heap_peek(MinHeapHandler_t * heap);
 
 /*!
  * \brief Clear the heap removing all elements
  *
  * \param heap The heap handler structure
  * \return MinHeapReturnCode
  *     - MIN_HEAP_NULL_POINTER if the heap handler is NULL
  *     - MIN_HEAP_OK otherwise
  */
 MinHeapReturnCode min_heap_clear(MinHeapHandler_t * heap);
 
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
 MinHeapReturnCode min_heap_insert(MinHeapHandler_t * heap, void * item);
 
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
 MinHeapReturnCode min_heap_remove(MinHeapHandler_t * heap, size_t index, void * out);
 
 /*!
  * \brief Find the index of an item in the heap array
  * \details This function has linear time complexity, use it wisely
  *
  * \param heap The heap handler structure
  * \param item The item to find
  * \return signed_size_t The index in the heap of the item if found, -1 otherwise
  */
 signed_size_t min_heap_find(MinHeapHandler_t * heap, void * item);
 
 
 #endif  // MIN_HEAP_API_H