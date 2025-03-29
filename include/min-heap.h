/*!
 * \file min-heap.h
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

#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*!
* \brief Structure definition used to pass the heap handler as a function parameter
* \attention This structure should not be used directly
*/
typedef struct {
    size_t data_size;
    size_t size;
    size_t capacity;
    int8_t (*compare)(void *, void *);
    void *data;
} MinHeapHandler_t;

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

typedef long signed_size_t;

#endif