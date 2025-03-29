/*!
 * \file min-heap.h
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

 #ifndef MIN_HEAP_H
 #define MIN_HEAP_H
 
 #include <stdint.h>
 #include <stddef.h>
 #include <stdbool.h>
 
 /*!
   * \brief Type definition for the min heap structure
   * \details As an example you can declare and initialize a new heap structure
   * that can contain 10 integers as follows:
   *      int8_t min_heap_compare(int a, int b) {
   *          if (a < b) return -1;
   *          return a == b ? 0 : 1;
   *      }
   *      MinHeap(int, 10) heap = min_heap_new(int, 10, min_heap_compare);
   *
   * \details The last item in the data array is used as swap storage when two items
   * in the array needs to be swapped
   *
   * \param TYPE The data type
   * \param CAPACITY The maximum number of elements of the heap
   *
   * \param data_size The size a single element in bytes
   * \param size The number of elements currently inside the heap
   * \param capacity The maximum number of elements that heap can contain
   * \param data The heap data
   * \param compare A pointer to a function that compares two items and returns:
   *     - -1: if the first item is less than the second
   *     -  0: if the items are equal
   *     -  1: if the first item is greater than the second
   */
 #define MinHeap(TYPE, CAPACITY)            \
     struct {                               \
         size_t data_size;                \
         size_t size;                       \
         size_t capacity;                   \
         int8_t (*compare)(void *, void *); \
         TYPE *data;           \
     }
 
 /*!
   * \brief Min heap structure initialization
   * \attention The TYPE and CAPACITY variables must be the same as the ones
   * in the structure declaration above
   *
   * \details As an example you can declare and initialize a new heap structure
   * that can contain 10 integers as follows:
   *      int8_t min_heap_compare(int a, int b) {
   *          if (a < b) return -1;
   *          return a == b ? 0 : 1;
   *      }
   *      MinHeap(int, 10) heap = min_heap_new(int, 10, min_heap_compare);
   *
   * \details If the min_heap_init function is used this macro is not needed
   *
   * \param TYPE The data type
   * \param CAPACITY The maximum number of elements of the heap
   * \param CMP_CALLBACK The callback function used to compare items in the heap
   */
 #define min_heap_new(TYPE, CAPACITY, CMP_CALLBACK) \
     {                                              \
         .data_size = sizeof(TYPE),                 \
         .size = 0,                                 \
         .capacity = CAPACITY,                      \
         .compare = CMP_CALLBACK,                   \
         .data = NULL                             \
     }
 
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
 } MinHeapInterface;
 
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