 /*!
 * \file min-heap.h
 * \date 2025-03-28
 * \authors Antonio Gelain [antonio.gelain2@gmail.com]
 * \authors Dorijan Di Zepp [dorijan.dizepp@eagletrt.it]
 * 
 * \brief Library that implements a minimum heap with an arena allocator
 * \details TODO
 * 
 * \warning TODO
 */

 #ifndef MIN_HEAP_H
 #define MIN_HEAP_H
 
 #include "arena-allocator.h"
 #include <stdint.h>
 
 /*!
  * \brief Enum with all the possible return codes for the min heap functions
  */
 typedef enum {
     MIN_HEAP_OK,
     MIN_HEAP_NULL_POINTER,
     MIN_HEAP_EMPTY,
     MIN_HEAP_FULL,
     MIN_HEAP_OUT_OF_BOUNDS
 } MinHeapReturnCode;
 
 /*!
  * \brief Structure definition used to pass the heap handler as a function parameter
  */
 typedef struct _MinHeapHandler_t {
     size_t capacity;
     int8_t (* compare)(void *, void *);
     ArenaAllocatorHandler_t arenaAllocator;
 } MinHeapHandler_t;
 
 #endif