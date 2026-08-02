/*!
 * \file min-heap.h
 * \date 2026-03-18
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
 * \brief Internal state handler for the minimum heap
 */
struct MinHeapHandler {
    size_t data_size;                  /*!< The size of a single item in bytes */
    size_t size;                       /*!< The number of elements contained in the heap */
    size_t capacity;                   /*!< The maximum number of elements that can be contained in the heap */
    int8_t (*compare)(void *, void *); /*!< The function used to compare two elements */
    void *data;                        /*!< The buffer containing the data */
};

/*!
 * \brief Enum with all the possible return codes for the min heap functions
 */
enum MinHeapReturnCode {
    MIN_HEAP_RC_OK,           /*!< Operation completed successfully */
    MIN_HEAP_RC_NULL_POINTER, /*!< A provided pointer was NULL */
    MIN_HEAP_RC_EMPTY,        /*!< The heap is empty; no elements to extract or peek */
    MIN_HEAP_RC_FULL,         /*!< The heap is at maximum capacity; cannot insert more elements */
    MIN_HEAP_RC_OUT_OF_BOUNDS /*!< The requested operation exceeds the heap boundaries */
};

#endif
