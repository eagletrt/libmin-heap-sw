/*!
* \file min-heap.h
* \date 2025-03-28
* \authors Antonio Gelain [antonio.gelain@studenti.unitn.it]
* \authors Dorijan Di Zepp [dorijan.dizepp@eagletrt.it]
*
* \brief Library that defines the structure of a min heap
* 
* \details A min heap is a binary heap where the smallest element is 
*      always at the root. It maintains the heap property, meaning each 
*      parent node has a value less than or equal to its children. 
*      It is also a complete binary tree, meaning all levels are fully 
*      filled except possibly the last, which is filled from left to right. 
*      The main operations include insertion, deletion and top value. 
*/

#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*!
 * \struct MinHeapHandler_t
 *
 * \var size_t data_size
 *       The size of a single item in bytes
 *
 * \var size_t size
 *       The number of elements contained in the heap
 *
 * \var size_t capacity
 *       The maximum number of elements that can be contained in the heap
 *
 * \var int8_t (*compare)(void *, void*)
 *       The function used to compare two element 
 *
 * \var void *data
 *       The buffer containign the data
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