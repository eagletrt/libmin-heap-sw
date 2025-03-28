#include "unity.h"
#include "min-heap-api.h"
#include <stdio.h>
#include <malloc.h>

MinHeapHandler_t heap;
MinHeapReturnCode returnCode;

/*!
 * \brief a function that compares two integer values and determines which one is 
 * the smallest
 * 
 * \param a void pointer to the first integer value
 * \param b void pointer to the second integer value
 * 
 * \return -1 if the first element is less than the second one
 * 1 if the first element is greater than the second
 * 0 if they are the same
 */
int8_t compare_ints(void* a, void* b) {
    int int_a = *(int *)a;
    int int_b = *(int *)b;
    return (int_a > int_b) - (int_a < int_b);
}

void setUp(void) {
    min_heap_init(&heap, 10, &compare_ints);
}

void tearDown(void) {
    min_heap_clear(&heap);
}

void test_min_heap_clear_api(void){
    int *value = malloc(sizeof(int));
    *value = 1;
    min_heap_insert(&heap, value);

    value = malloc(sizeof(int));
    *value = 2;
    min_heap_insert(&heap, value);

    value = malloc(sizeof(int));
    *value = 3;
    min_heap_insert(&heap, value);
    min_heap_clear(&heap);
    
    int *topElem;
    TEST_ASSERT_EQUAL(MIN_HEAP_EMPTY, min_heap_top(&heap, topElem));
}

void test_min_heap_api_insert(void){
    int value = 5;
    returnCode = min_heap_insert(&heap, &value);

    TEST_ASSERT_EQUAL(MIN_HEAP_OK, returnCode);

    int value_2 = 10;
    returnCode =  min_heap_insert(&heap, &value_2);

    TEST_ASSERT_EQUAL(MIN_HEAP_OK, returnCode);
}

void test_min_heap_empty_api_not(void){
    int value = 5;
    returnCode = min_heap_insert(&heap, &value);

    bool isEmpty = min_heap_is_empty(&heap);
    TEST_ASSERT_EQUAL(false, isEmpty);
}

void test_min_heap_size_api(void){
    int value = 1;
    for (int i = 0; i < 7; i++)
    {
        min_heap_insert(&heap, &value);
    }

    TEST_ASSERT_EQUAL(7, min_heap_size(&heap));
}

void test_min_heap_top_api(void){
    //insert two values that need to be swapped
    int value = 10;
    min_heap_insert(&heap, &value);
    
    int value_2 = 5;
    min_heap_insert(&heap, &value_2);
    
    //check that the top item is the smallest one
    int *topItem = malloc(sizeof(int));
    min_heap_top(&heap, topItem);
    TEST_ASSERT_EQUAL(5, *topItem);
    free(topItem);
}

void test_min_heap_remove_api(void){
    int value = 5;
    min_heap_insert(&heap, &value);
    
    int value_2 = 10;
    min_heap_insert(&heap, &value_2);
    
    min_heap_remove(&heap, 0U, NULL);
    
    int *topItem = malloc(sizeof(int));
    min_heap_top(&heap, topItem);

    TEST_ASSERT_EQUAL(10, *topItem);
    free(topItem);
}

void test_min_heap_insert_api_multiple_elements(void){
    int* value;
    for (int i = 0; i < 10; i++)
    {
        value = malloc(sizeof(int));
        *value = 10-i;
        min_heap_insert(&heap, value);
    }
    
    int *topItem = malloc(sizeof(int));
    min_heap_top(&heap, topItem);
    TEST_ASSERT_EQUAL(1, *topItem);
    free(topItem);
}

void test_min_heap_is_full_api(void){
    int value;
    for (int i = 0; i < 10; i++)
    {
        value = 10-i;
        min_heap_insert(&heap, &value);
    }
    
    TEST_ASSERT_EQUAL(true, min_heap_is_full(&heap));
}

void test_min_heap_is_full_api_more_data(void){
    int value;
    for (int i = 0; i < 20; i++)
    {
        value = 20-i;
        min_heap_insert(&heap, &value);
    }
    
    int *topItem = malloc(sizeof(int));
    min_heap_top(&heap, &topItem);
    TEST_ASSERT_EQUAL(10, *topItem);
    free(topItem);
}

void test_min_heap_multiple_insert_remove(void){
    int *topItem = malloc(sizeof(int));
    int *value = malloc(sizeof(int));

    *value = 1;
    min_heap_insert(&heap, value);

    value = malloc(sizeof(int));
    *value = 2;
    min_heap_insert(&heap, value);

    value = malloc(sizeof(int));
    *value = 3;
    min_heap_insert(&heap, value);

    min_heap_remove(&heap, 2U, topItem);  // Remove the item at index 2

    value = malloc(sizeof(int));
    *value = 4;
    min_heap_insert(&heap, value);

    value = malloc(sizeof(int));
    *value = 5;
    min_heap_insert(&heap, value);

    min_heap_remove(&heap, 1U, topItem);  // Remove the item at index 1

    min_heap_remove(&heap, 0U, topItem);  // Remove the item at index 0

    value = malloc(sizeof(int));
    *value = 6;
    min_heap_insert(&heap, value);  // Insert new value

    min_heap_top(&heap, topItem);
    TEST_ASSERT_EQUAL(4, *topItem);  // Check if the top item is 4

    free(value);
    free(topItem);
}

void test_min_heap_find_api(void){
    int *value = malloc(sizeof(int)*3);
    value[0] = 4;
    value[1] = 2;
    value[2] = 0;

    min_heap_insert(&heap, &value[0]);

    min_heap_insert(&heap, &value[1]);

    min_heap_insert(&heap, &value[2]);

    int find = 4;
    signed_size_t index = min_heap_find(&heap, &find);
    TEST_ASSERT_EQUAL(1, index);
    free(value);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_min_heap_clear_api);

    RUN_TEST(test_min_heap_size_api);

    RUN_TEST(test_min_heap_api_insert);

    RUN_TEST(test_min_heap_empty_api_not);

    RUN_TEST(test_min_heap_top_api);

    RUN_TEST(test_min_heap_remove_api);

    RUN_TEST(test_min_heap_insert_api_multiple_elements);

    RUN_TEST(test_min_heap_is_full_api);

    RUN_TEST(test_min_heap_multiple_insert_remove);

    RUN_TEST(test_min_heap_find_api);

    return UNITY_END();
}