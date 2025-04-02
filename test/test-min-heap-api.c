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

#include "unity.h"
#include "min-heap-api.h"

typedef struct {
    float x, y;
} Point;

int8_t min_heap_compare_float(void *f, void *s) {
    float a = *(float *)f;
    float b = *(float *)s;
    if (a < b)
        return -1;
    return a == b ? 0 : 1;
}
int8_t min_heap_compare_int(void *f, void *s) {
    int a = *(int *)f;
    int b = *(int *)s;
    if (a < b)
        return -1;
    return a == b ? 0 : 1;
}
int8_t min_heap_compare_point(void *f, void *s) {
    Point *a = (Point *)f;
    Point *b = (Point *)s;
    size_t dist_a = a->x * a->x + a->y * a->y;
    size_t dist_b = b->x * b->x + b->y * b->y;
    if (dist_a < dist_b)
        return -1;
    return dist_a == dist_b ? 0 : 1;
}

MinHeapHandler_t int_heap;
MinHeapHandler_t point_heap;
ArenaAllocatorHandler_t arena;

void setUp(void) {
    arena_allocator_api_init(&arena);
    min_heap_api_init(&int_heap, sizeof(int), 10, min_heap_compare_int, &arena);
    min_heap_api_init(&point_heap, sizeof(Point), 10, min_heap_compare_point, &arena);
}

void tearDown(void) {
    min_heap_api_clear(&int_heap);
    min_heap_api_clear(&point_heap);
    arena_allocator_api_free(&arena);
}

/*!
 * \defgroup min_heap_api_init Test heap min initialization
 * @{
 */

void check_min_heap_api_init_with_null_handler(void) {
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_init(NULL, sizeof(float), 3, min_heap_compare_float, &arena));
}
void check_min_heap_api_init_with_null_arena(void) {
    MinHeapHandler_t heap;
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_init(&heap, sizeof(float), 3, min_heap_compare_float, NULL));
}
void check_min_heap_api_init_with_null_callback(void) {
    MinHeapHandler_t heap;
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_init(&heap, sizeof(float), 3, NULL, &arena));
}
void check_min_heap_api_init(void) {
    MinHeapHandler_t heap;
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_OK, min_heap_api_init(&heap, sizeof(float), 3, min_heap_compare_float, &arena));
}

/*! @} */

/*!
 * \defgroup min_heap_api_size Test min heap size function
 * @{
 */

void check_min_heap_api_size_null(void) {
    TEST_ASSERT_EQUAL_size_t(min_heap_api_size(NULL), 0U);
}
void check_min_heap_api_size_empty(void) {
    TEST_ASSERT_EQUAL_size_t(min_heap_api_size(&int_heap), 0U);
}
void check_min_heap_api_size_not_empty(void) {
    // The size depends exclusively on the size variable
    int_heap.size = 3;
    TEST_ASSERT_EQUAL_size_t(min_heap_api_size(&int_heap), 3U);
}

/*! @} */

/*!
 * \defgroup min_heap_api_is_empty Test min heap empty function
 * @{
 */

void check_min_heap_api_empty_with_null(void) {
    TEST_ASSERT_TRUE(min_heap_api_is_empty(NULL));
}
void check_min_heap_api_empty_when_empty(void) {
    TEST_ASSERT_TRUE(min_heap_api_is_empty(&int_heap));
}
void check_min_heap_api_empty_when_not_empty(void) {
    // The empty check depends exclusively on the size variable
    int_heap.size = 3;
    TEST_ASSERT_FALSE(min_heap_api_is_empty(&int_heap));
}

/*! @} */

/*!
 * \defgroup min_heap_api_full Test min heap fullness function
 * @{
 */
void check_min_heap_api_full_with_null(void) {
    TEST_ASSERT_TRUE(min_heap_api_is_full(NULL));
}
void check_min_heap_api_full_when_full(void) {
    // The full check depends exclusively on the size variable
    int_heap.size = int_heap.capacity;
    TEST_ASSERT_TRUE(min_heap_api_is_full(&int_heap));
}
void check_min_heap_api_full_when_not_full(void) {
    TEST_ASSERT_FALSE(min_heap_api_is_full(&int_heap));
}

/*! @} */

/*!
 * \defgroup min_heap_api_top Test min heap top function
 * @{
 */

void check_min_heap_api_top_with_null_data_heap(void) {
    MinHeapHandler_t heap;
    heap.data = NULL;
    int a;
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_top(&heap, &a));
}
void check_min_heap_api_top_with_null_heap(void) {
    int a;
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_top(NULL, &a));
}
void check_min_heap_api_top_with_null_item(void) {
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_top(&int_heap, NULL));
}
void check_min_heap_api_top_when_empty(void) {
    int a;
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_EMPTY, min_heap_api_top(&int_heap, &a));
}
void check_min_heap_api_top_when_not_empty_return_value(void) {
    const int val = 7;
    int_heap.size = 1;
    ((int *)int_heap.data)[0] = val;
    int a;
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_OK, min_heap_api_top(&int_heap, &a));
}
void check_min_heap_api_top_when_not_empty_data(void) {
    const int val = 7;
    int_heap.size = 1;
    ((int *)int_heap.data)[0] = val;
    int a = 0;
    min_heap_api_top(&int_heap, &a);
    TEST_ASSERT_EQUAL_INT(val, a);
}

/*! @} */

/*!
 * \defgroup min_heap_api_peek Test min heap peek function
 * @{
 */

void check_min_heap_api_peek_with_null(void) {
    TEST_ASSERT_NULL(min_heap_api_peek(NULL));
}
void check_min_heap_api_peek_when_empty(void) {
    TEST_ASSERT_NULL(min_heap_api_peek(&int_heap));
}
void check_min_heap_api_peek_when_not_empty(void) {
    const int val = 5;
    int_heap.size = 1;
    ((int *)int_heap.data)[0] = val;
    int *item = (int *)min_heap_api_peek(&int_heap);
    TEST_ASSERT_NOT_NULL_MESSAGE(item, "The pointer to the first item is NULL");
    TEST_ASSERT_NOT_EQUAL_HEX64_MESSAGE(item, &val, "The address of the item is the same as the input variable");
    TEST_ASSERT_EQUAL_INT_MESSAGE(*item, val, "Value in the int_heap is differen from the input value");
}

/*! @} */

/*!
 * \defgroup min_heap_api_clear Test mean heap clear function
 * @{
 */

void check_min_heap_api_clear_with_null(void) {
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_clear(NULL));
}
void check_min_heap_api_clear_return_value(void) {
    // The clear function resets only the size of the int_heap
    int_heap.size = 3;
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_OK, min_heap_api_clear(&int_heap));
}
void check_min_heap_api_clear_size(void) {
    // The clear function resets only the size of the int_heap
    int_heap.size = 3;
    min_heap_api_clear(&int_heap);
    TEST_ASSERT_EQUAL_size_t(int_heap.size, 0U);
}

/*! @} */

/*!
 * \defgroup min_heap_api_insert Test min heap insert function
 * @{
 */

void check_min_heap_api_insert_with_null_handler(void) {
    Point p = { .x = 5.4f, .y = 2.7f };
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_insert(NULL, &p));
}
void check_min_heap_api_insert_with_null_compare(void) {
    Point p = { .x = 5.4f, .y = 2.7f };
    point_heap.compare = NULL;
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_insert(&point_heap, &p));
}
void check_min_heap_api_insert_with_null_item(void) {
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_insert(&point_heap, NULL));
}
void check_min_heap_api_insert_with_null_data(void) {
    point_heap.data = NULL;
    Point p = { .x = 5.4f, .y = 2.7f };
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_insert(&point_heap, &p));
}
void check_min_heap_api_insert_when_full(void) {
    Point p = { .x = 5.4f, .y = 2.7f };
    point_heap.size = point_heap.capacity;
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_FULL, min_heap_api_insert(&point_heap, &p));
}
void check_min_heap_api_insert_return_value(void) {
    Point p = { .x = 5.4f, .y = 2.7f };
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_OK, min_heap_api_insert(&point_heap, &p));
}
void check_min_heap_api_insert_size(void) {
    Point p = { .x = 5.4f, .y = 2.7f };
    min_heap_api_insert(&point_heap, &p);
    TEST_ASSERT_EQUAL_size_t(1U, point_heap.size);
}
void check_min_heap_api_insert_top_data(void) {
    Point p = { .x = 5.4f, .y = 2.7f };
    min_heap_api_insert(&point_heap, &p);
    TEST_ASSERT_EQUAL_MEMORY(&p, point_heap.data, sizeof(Point));
}
void check_min_heap_api_insert_middle_data(void) {
    // Insert at the top a point that is greater then the next
    Point p1 = { .x = 5.4f, .y = 2.7f };
    Point p2 = { .x = 10.f, .y = 10.f };

    point_heap.size = 1;
    ((Point *)point_heap.data)[0] = p2;

    min_heap_api_insert(&point_heap, &p1);
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(&p1, ((Point *)point_heap.data), sizeof(Point), "First item has different data");
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(&p2, ((Point *)point_heap.data) + 1, sizeof(Point), "Second item has different data");
}
void check_min_heap_api_insert_middle_data_in_order(void) {
    // Insert at the top a point that is less then the next
    Point p1 = { .x = 5.4f, .y = 2.7f };
    Point p2 = { .x = 10.f, .y = 10.f };

    point_heap.size = 1;
    ((Point *)point_heap.data)[0] = p1;

    min_heap_api_insert(&point_heap, &p2);
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(&p1, ((Point *)point_heap.data), sizeof(Point), "First item has different data");
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(&p2, ((Point *)point_heap.data) + 1, sizeof(Point), "Second item has different data");
}

/*! @} */

/*!
 * \defgroup min_heap_api_remove Test min heap remove function
 * @{
 */

void check_min_heap_api_remove_with_null_handler(void) {
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_remove(NULL, 0, NULL));
}
void check_min_heap_api_remove_with_null_compare(void) {
    point_heap.compare = NULL;
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_NULL_POINTER, min_heap_api_remove(&point_heap, 0, NULL));
}
void check_min_heap_api_remove_when_empty(void) {
    TEST_ASSERT_EQUAL_INT(MIN_HEAP_EMPTY, min_heap_api_remove(&point_heap, 0, NULL));
}
void check_min_heap_api_remove_out_of_bounds(void) {
    Point p1 = { .x = 5.4f, .y = 2.7f };

    point_heap.size = 1;
    ((Point *)point_heap.data)[0] = p1;

    TEST_ASSERT_EQUAL_INT(MIN_HEAP_OUT_OF_BOUNDS, min_heap_api_remove(&point_heap, 10, NULL));
}
void check_min_heap_api_remove_return_value(void) {
    Point p1 = { .x = 5.4f, .y = 2.7f };

    point_heap.size = 1;
    ((Point *)point_heap.data)[0] = p1;

    TEST_ASSERT_EQUAL_INT(MIN_HEAP_OK, min_heap_api_remove(&point_heap, 0, NULL));
}
void check_min_heap_api_remove_removed_item_data(void) {
    Point p1 = { .x = 5.4f, .y = 2.7f };
    Point removed;

    point_heap.size = 1;
    ((Point *)point_heap.data)[0] = p1;

    min_heap_api_remove(&point_heap, 0, &removed);
    TEST_ASSERT_EQUAL_MEMORY(&p1, &removed, sizeof(Point));
}
void check_min_heap_api_remove_size(void) {
    Point p1 = { .x = 5.4f, .y = 2.7f };
    Point p2 = { .x = 10.f, .y = 10.f };

    point_heap.size = 2;
    ((Point *)point_heap.data)[0] = p1;
    ((Point *)point_heap.data)[1] = p2;

    min_heap_api_remove(&point_heap, 1, NULL);
    TEST_ASSERT_EQUAL_size_t(1, point_heap.size);
}
void check_min_heap_api_remove_leaf_data(void) {
    Point p1 = { .x = 5.4f, .y = 2.7f };
    Point p2 = { .x = 10.f, .y = 10.f };

    point_heap.size = 2;
    ((Point *)point_heap.data)[0] = p1;
    ((Point *)point_heap.data)[1] = p2;

    min_heap_api_remove(&point_heap, 1, NULL);
    // Check if other item is unchanged
    TEST_ASSERT_EQUAL_MEMORY(&p1, &((Point *)point_heap.data)[0], point_heap.data_size);
}
void check_min_heap_api_remove_equal_data(void) {
    Point root = { .x = 5.4f, .y = 2.7f };
    Point r = { .x = 5.4f, .y = 2.7f };
    Point l = { .x = 10.f, .y = 10.f };

    point_heap.size = 3;
    ((Point *)point_heap.data)[0] = root;
    ((Point *)point_heap.data)[1] = l;
    ((Point *)point_heap.data)[2] = r;

    min_heap_api_remove(&point_heap, 0, NULL);
    TEST_ASSERT_EQUAL_MEMORY(&r, &((Point *)point_heap.data)[0], point_heap.data_size);
}
void check_min_heap_api_remove_up_heapify_data(void) {
    // The swapped element is lower than the last item in the heap (
    Point root = { .x = 1.f, .y = 1.f };
    Point l = { .x = 10.f, .y = 10.f };
    Point r = { .x = 1.4f, .y = 1.3f };

    Point ll = { .x = 11.f, .y = 11.f };
    Point lr = { .x = 12.f, .y = 12.f };
    Point rl = { .x = 2.f, .y = 2.f };
    Point rr = { .x = 3.14f, .y = 2.5f };

    point_heap.size = 7;
    ((Point *)point_heap.data)[0] = root;
    ((Point *)point_heap.data)[1] = l;
    ((Point *)point_heap.data)[2] = r;
    ((Point *)point_heap.data)[3] = ll;
    ((Point *)point_heap.data)[4] = lr;
    ((Point *)point_heap.data)[5] = rl;
    ((Point *)point_heap.data)[6] = rr;

    // After the deletion of the 'll' node the 'l' and the 'rr' nodes should swap
    Point expected[6] = { root, rr, r, l, lr, rl };

    min_heap_api_remove(&point_heap, 3, NULL);
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(expected, point_heap.data, sizeof(Point), 6);
}
void check_min_heap_api_remove_down_heapify_data(void) {
    // The swapped element is lower than the last item in the heap
    Point root = { .x = 1.f, .y = 1.f };
    Point l = { .x = 1.4f, .y = 1.3f };
    Point r = { .x = 10.f, .y = 10.f };

    Point ll = { .x = 2.f, .y = 2.f };
    Point lr = { .x = 3.14f, .y = 2.5f };
    Point rl = { .x = 11.f, .y = 11.f };
    Point rr = { .x = 12.f, .y = 12.f };

    point_heap.size = 7;
    ((Point *)point_heap.data)[0] = root;
    ((Point *)point_heap.data)[1] = l;
    ((Point *)point_heap.data)[2] = r;
    ((Point *)point_heap.data)[3] = ll;
    ((Point *)point_heap.data)[4] = lr;
    ((Point *)point_heap.data)[5] = rl;
    ((Point *)point_heap.data)[6] = rr;

    // After the deletion of the 'root' node the 'll' and the 'rr' nodes should swap
    Point expected[6] = { l, ll, r, rr, lr, rl };

    min_heap_api_remove(&point_heap, 0, NULL);
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(expected, point_heap.data, point_heap.data_size, 6);
}
void check_min_heap_api_remove_not_heapify_data(void) {
    Point root = { .x = 1.f, .y = 1.f };
    Point l = { .x = 1.4f, .y = 1.3f };
    Point r = { .x = 10.f, .y = 10.f };

    Point ll = { .x = 2.f, .y = 2.f };
    Point lr = { .x = 3.14f, .y = 2.5f };
    Point rl = { .x = 11.f, .y = 11.f };
    Point rr = { .x = 12.f, .y = 12.f };

    Point lll = { .x = 6.f, .y = 6.f };
    Point llr = { .x = 7.f, .y = 7.f };

    point_heap.size = 9;
    ((Point *)point_heap.data)[0] = root;
    ((Point *)point_heap.data)[1] = l;
    ((Point *)point_heap.data)[2] = r;
    ((Point *)point_heap.data)[3] = ll;
    ((Point *)point_heap.data)[4] = lr;
    ((Point *)point_heap.data)[5] = rl;
    ((Point *)point_heap.data)[6] = rr;
    ((Point *)point_heap.data)[7] = lll;
    ((Point *)point_heap.data)[8] = llr;

    min_heap_api_remove(&point_heap, 3, NULL);

    // After the deletion of the 'root' node the new root is expected to be 'r'
    Point expected[8] = { root, l, r, llr, lr, rl, rr, lll };

    min_heap_api_remove(&point_heap, 3, NULL);
    TEST_ASSERT_EQUAL_MEMORY_ARRAY(expected, point_heap.data, sizeof(Point), 8);
}
void check_min_heap_api_remove_root_not_down_heapify_data(void) {
    Point root = { .x = 7.f, .y = 7.f };
    Point l = { .x = 8.f, .y = 8.f };
    Point r = { .x = 7.f, .y = 7.f };

    Point ll = { .x = 3.f, .y = 3.f };
    Point lr = { .x = 4.f, .y = 4.f };
    Point rl = { .x = 5.f, .y = 5.f };
    Point rr = { .x = 6.f, .y = 6.f };

    Point lll = { .x = 4.f, .y = 4.f };
    Point llr = { .x = 4.f, .y = 4.f };
    Point lrl = { .x = 5.f, .y = 5.f };
    Point lrr = { .x = 5.f, .y = 5.f };
    Point rll = { .x = 11.f, .y = 11.f };
    Point rlr = { .x = 11.f, .y = 11.f };
    Point rrl = { .x = 7.f, .y = 7.f };
    Point rrr = { .x = 10.f, .y = 10.f };

    MinHeapHandler_t point_heap;
    min_heap_api_init(&point_heap, sizeof(Point), 16, min_heap_compare_point, &arena);
    point_heap.size = 15;

    ((Point *)point_heap.data)[0] = root;
    ((Point *)point_heap.data)[1] = l;
    ((Point *)point_heap.data)[2] = r;
    ((Point *)point_heap.data)[3] = ll;
    ((Point *)point_heap.data)[4] = lr;
    ((Point *)point_heap.data)[5] = rl;
    ((Point *)point_heap.data)[6] = rr;
    ((Point *)point_heap.data)[7] = lll;
    ((Point *)point_heap.data)[8] = llr;
    ((Point *)point_heap.data)[9] = lrl;
    ((Point *)point_heap.data)[10] = lrr;
    ((Point *)point_heap.data)[11] = rll;
    ((Point *)point_heap.data)[12] = rlr;
    ((Point *)point_heap.data)[13] = rrl;
    ((Point *)point_heap.data)[14] = rrr;

    Point expected[14] = { r, l, rl, ll, lr, rrr, rr, lll, llr, lrl, lrr, rll, rlr, rrl };

    min_heap_api_remove(&point_heap, 0, NULL);

    TEST_ASSERT_EQUAL_MEMORY_ARRAY(expected, point_heap.data, sizeof(Point), 14);
}
void check_min_heap_api_remove_root_up_heapify_data(void){
    Point root = { .x = 2.f, .y = 2.f };
    Point l = { .x = 5.f, .y = 5.f };  
    Point r = { .x = 4.f, .y = 4.f };  
    Point ll = { .x = 7.f, .y = 7.f }; 
    Point lr = { .x = 3.f, .y = 3.f }; 
    Point rl = { .x = 8.f, .y = 8.f };
    Point rr = { .x = 1.f, .y = 1.f };

    point_heap.size = 7;
    ((Point *)point_heap.data)[0] = root;
    ((Point *)point_heap.data)[1] = l;   
    ((Point *)point_heap.data)[2] = r;   
    ((Point *)point_heap.data)[3] = ll;  
    ((Point *)point_heap.data)[4] = lr;  
    ((Point *)point_heap.data)[5] = rl;
    ((Point *)point_heap.data)[6] = rr;

    min_heap_api_remove(&point_heap, 2, NULL);
    Point expected[6] = { rr, l, root, ll, lr, rl };

    TEST_ASSERT_EQUAL_MEMORY_ARRAY(expected, point_heap.data, sizeof(Point), 6);
}

/*! @} */

/*!
 * \defgroup min_heap_api_find Test min heap find function
 * @{
 */

void check_min_heap_api_find_with_null_heap(void) {
    int a = 0;
    TEST_ASSERT_LESS_THAN_INT(0, min_heap_api_find(NULL, &a));
}
void check_min_heap_api_find_with_null_item(void) {
    TEST_ASSERT_LESS_THAN_INT(0, min_heap_api_find(&int_heap, NULL));
}
void check_min_heap_api_find_when_empty(void) {
    int a = 0;
    TEST_ASSERT_LESS_THAN_INT(0, min_heap_api_find(&int_heap, &a));
}
void check_min_heap_api_find_fail(void) {
    int_heap.size = 3;
    ((int *)int_heap.data)[0] = 7;
    ((int *)int_heap.data)[1] = 3;
    ((int *)int_heap.data)[2] = 6;
    int a = 2;
    TEST_ASSERT_LESS_THAN_INT(0, min_heap_api_find(&int_heap, &a));
}
void check_min_heap_api_find_success(void) {
    int_heap.size = 3;
    ((int *)int_heap.data)[0] = 7;
    ((int *)int_heap.data)[1] = 3;
    ((int *)int_heap.data)[2] = 6;
    int a = 3;
    TEST_ASSERT_EQUAL(1, min_heap_api_find(&int_heap, &a));
}
void check_min_heap_api_find_when_null_compare(void) {
    int_heap.compare = NULL;
    int a = 0;
    TEST_ASSERT_LESS_THAN_INT(0, min_heap_api_find(&int_heap, &a));
}
void check_min_heap_api_find_when_null_data(void) {
    int_heap.data = NULL;
    int_heap.size = 1;
    int a = 0;
    TEST_ASSERT_LESS_THAN_INT(0, min_heap_api_find(&int_heap, &a));
}

/*! @} */

int main() {
    UNITY_BEGIN();

    /*!
     * \addtogroup min_heap_api_init Run test for mean heap initialization
     * @{
     */

    RUN_TEST(check_min_heap_api_init_with_null_handler);
    RUN_TEST(check_min_heap_api_init_with_null_arena);
    RUN_TEST(check_min_heap_api_init_with_null_callback);
    RUN_TEST(check_min_heap_api_init);

    /*! @} */

    /*!
     * \addtogroup min_heap_api_size Run test for mean heap size function
     * @{
     */

    RUN_TEST(check_min_heap_api_size_null);
    RUN_TEST(check_min_heap_api_size_empty);
    RUN_TEST(check_min_heap_api_size_not_empty);

    /*! @} */

    /*!
     * \addtogroup min_heap_api_is_empty Run test for min heap empty function
     * @{
     */

    RUN_TEST(check_min_heap_api_empty_with_null);
    RUN_TEST(check_min_heap_api_empty_when_empty);
    RUN_TEST(check_min_heap_api_empty_when_not_empty);

    /*! @} */

    /*!
     * \defgroup min_heap_api_full Run test for min heap fullness function
     * @{
     */

    RUN_TEST(check_min_heap_api_full_with_null);
    RUN_TEST(check_min_heap_api_full_when_full);
    RUN_TEST(check_min_heap_api_full_when_not_full);

    /*! @} */

    /*!
     * \addtogroup min_heap_api_top Run test for min heap top function
     * @{
     */

    RUN_TEST(check_min_heap_api_top_with_null_data_heap);
    RUN_TEST(check_min_heap_api_top_with_null_heap);
    RUN_TEST(check_min_heap_api_top_with_null_item);
    RUN_TEST(check_min_heap_api_top_when_empty);
    RUN_TEST(check_min_heap_api_top_when_not_empty_return_value);
    RUN_TEST(check_min_heap_api_top_when_not_empty_data);

    /*! @} */

    /*!
     * \addtogroup min_heap_api_peek Run test for min heap peek function
     * @{
     */

    RUN_TEST(check_min_heap_api_peek_with_null);
    RUN_TEST(check_min_heap_api_peek_when_empty);
    RUN_TEST(check_min_heap_api_peek_when_not_empty);

    /*! @} */

    /*!
     * \addtogroup min_heap_api_clear Run test for mean heap clear function
     * @{
     */

    RUN_TEST(check_min_heap_api_clear_with_null);
    RUN_TEST(check_min_heap_api_clear_return_value);
    RUN_TEST(check_min_heap_api_clear_size);

    /*! @} */

    /*!
     * \addtogroup min_heap_api_insert Run test for min heap insert function
     * @{
     */

    RUN_TEST(check_min_heap_api_insert_with_null_handler);
    RUN_TEST(check_min_heap_api_insert_with_null_compare);
    RUN_TEST(check_min_heap_api_insert_with_null_item);
    RUN_TEST(check_min_heap_api_insert_with_null_data);
    RUN_TEST(check_min_heap_api_insert_when_full);
    RUN_TEST(check_min_heap_api_insert_return_value);
    RUN_TEST(check_min_heap_api_insert_size);
    RUN_TEST(check_min_heap_api_insert_top_data);
    RUN_TEST(check_min_heap_api_insert_middle_data);
    RUN_TEST(check_min_heap_api_insert_middle_data_in_order);

    /*! @} */

    /*!
     * \addtogroup min_heap_api_remove Run test for min heap remove function
     * @{
     */

    RUN_TEST(check_min_heap_api_remove_with_null_handler);
    RUN_TEST(check_min_heap_api_remove_with_null_compare);
    RUN_TEST(check_min_heap_api_remove_when_empty);
    RUN_TEST(check_min_heap_api_remove_out_of_bounds);
    RUN_TEST(check_min_heap_api_remove_return_value);
    RUN_TEST(check_min_heap_api_remove_removed_item_data);
    RUN_TEST(check_min_heap_api_remove_size);
    RUN_TEST(check_min_heap_api_remove_leaf_data);
    RUN_TEST(check_min_heap_api_remove_equal_data);
    RUN_TEST(check_min_heap_api_remove_up_heapify_data);
    RUN_TEST(check_min_heap_api_remove_down_heapify_data);
    RUN_TEST(check_min_heap_api_remove_not_heapify_data);
    RUN_TEST(check_min_heap_api_remove_root_not_down_heapify_data);
    RUN_TEST(check_min_heap_api_remove_root_up_heapify_data);

    /*! @} */

    /*!
     * \addtogroup min_heap_api_find Run test for min heap find function
     * @{
     */

    RUN_TEST(check_min_heap_api_find_with_null_heap);
    RUN_TEST(check_min_heap_api_find_with_null_item);
    RUN_TEST(check_min_heap_api_find_when_empty);
    RUN_TEST(check_min_heap_api_find_fail);
    RUN_TEST(check_min_heap_api_find_success);
    RUN_TEST(check_min_heap_api_find_when_null_compare);
    RUN_TEST(check_min_heap_api_find_when_null_data);

    /*! @} */

    UNITY_END();
}
