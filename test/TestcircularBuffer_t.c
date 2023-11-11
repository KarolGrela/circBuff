#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "unity.h"
#include "circularBuffer_t.h"

#define BUFFER_LEN 8

static char buffer[BUFFER_LEN];
struct circularBuffer_t circBuff;

/* These functions are intended to be called before and after each test.
 * If using unity directly, these will need to be provided for each test
 * executable built. If you are using the test runner generator and/or
 * Ceedling, these are optional. */
void setUp(void)
{
    
}

void tearDown(void)
{
    
}

void test_firstFailingTest(void)
{
    TEST_ASSERT(false);
}

///
/// circularBuffer_init
///

// check if init returns pointer to local circBuffer
void test_circularBuffer_init_return(void)
{
    struct circularBuffer_t* ptr_circBuff = circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    
    TEST_ASSERT_EQUAL_HEX32(&circBuff, ptr_circBuff);
}

// check if circBuffer point to char buffer
void test_circularBuffer_init_charBuffer(void)
{
    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    
    TEST_ASSERT_EQUAL_HEX32(buffer, circBuff.buff_origin);
}

// check if circBuffer head point to char buffer
void test_circularBuffer_init_head(void)
{
    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    
    TEST_ASSERT_EQUAL_HEX32(circBuff.head, circBuff.buff_origin);
}

// check if circBuffer head point to char buffer
void test_circularBuffer_init_tail(void)
{
    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    
    TEST_ASSERT_EQUAL_HEX32(circBuff.tail, circBuff.buff_origin);
}

// check if circBuffer has proper max size
void test_circularBuffer_init_size(void)
{
    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    
    TEST_ASSERT_EQUAL_UINT16(BUFFER_LEN, circBuff.size);
}

// check if circBuffer has proper max size
void test_circularBuffer_init_len(void)
{
    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    
    TEST_ASSERT_EQUAL_UINT16(0, circularBuffer_len(circBuff));
}

///
/// circularBuffer_max_size
///

void test_circularBuffer_max_size(void)
{
    uint16_t expectedValue, observedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    expectedValue = BUFFER_LEN;
    observedValue = circularBuffer_max_size(circBuff);

    TEST_ASSERT_EQUAL_UINT16(expectedValue, observedValue);
}

///
/// circularBuffer_push
///

// check if push changes char buffer
void test_circularBuffer_push_one(void)
{
    char localStr[2] = "g";
    char expectedValue = 'g';

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    circularBuffer_push(&circBuff, localStr[0]);

    TEST_ASSERT_EQUAL_CHAR(expectedValue, *circBuff.tail);
}

// check if push moves head pointer
void test_circularBuffer_push_one_head(void)
{
    unsigned char* expectedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    expectedValue = circBuff.head + 1;

    circularBuffer_push(&circBuff, 'g');

    TEST_ASSERT_EQUAL_HEX32(expectedValue, circBuff.head);
}

// check if push moves tail pointer
void test_circularBuffer_push_one_tail(void)
{
    unsigned char* expectedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    expectedValue = circBuff.tail;

    circularBuffer_push(&circBuff, 'g');

    TEST_ASSERT_EQUAL_HEX32(expectedValue, circBuff.tail);
    TEST_ASSERT_EQUAL_HEX32(expectedValue, circBuff.buff_origin);
}

// check if using push multiple times changes char buffer
void test_circularBuffer_push_three(void)
{
    const size_t N = 4;
    char expectedString[4] = "abc";

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    for (size_t i = 0; i < N; i++)
    {
        circularBuffer_push(&circBuff, expectedString[i]);
    }
    
    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedString, circBuff.buff_origin, N);
}

// check if using push multiple times moves head
void test_circularBuffer_push_three_head(void)
{
    const size_t N = 4;
    char str[4] = "abc";
    unsigned char* expectedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    expectedValue = circBuff.head + N - 1;

    for (size_t i = 0; i < N - 1; i++)
    {
        circularBuffer_push(&circBuff, str[i]);
    }
    
    TEST_ASSERT_EQUAL_HEX32(expectedValue, circBuff.head);
}

// check if using push to overflow works correctly
void test_circularBuffer_push_overflow(void)
{
    const size_t N = 9;
    char str[9] = "abcdefgh";
    char expectedValue[8] = "abcdefg";

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    for (size_t i = 0; i < N; i++)
    {
        circularBuffer_push(&circBuff, str[i]);
    }
    
    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedValue, circBuff.buff_origin, circBuff.size - 1);
}

// check if push changes length
void test_circularBuffer_push_overflow_head(void)
{
    const size_t N = 9;
    char str[9] = "abcdefgh";
    char* expectedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    expectedValue = circBuff.head + circBuff.size - 1;

    for (size_t i = 0; i < N; i++)
    {
        circularBuffer_push(&circBuff, str[i]);
    }
    
    TEST_ASSERT_EQUAL_HEX32(expectedValue, circBuff.head);
}

// check if using push to overflow and check if contiguous memory is affected
void test_circularBuffer_push_overflow_memory(void)
{
    const size_t N = 11;
    char memcpyString[11]  = "abcdefgXXX";
    char pushString[11]    = "abcdefghij";
    char expectedValue[11] = "abcdefgXXX";

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    memcpy((void*)buffer, (void*)memcpyString, N);

    for (size_t i = 0; i < N; i++)
    {
        circularBuffer_push(&circBuff, pushString[i]);
    }
    
    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedValue, circBuff.buff_origin, N);
}

// check if push writes new data to first node after reaching last node
void test_circularBuffer_push_again(void)
{
    const size_t N = 7;
    char pushString[8] = "abcdefg";
    char pushAgain[3] = "hA";
    char expectedChar_0 = 'A';
    char expectedArray[9] = "Abcdefgh";

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    for (size_t i = 0; i < N; i++)
    {
        circularBuffer_push(&circBuff, pushString[i]);
    }

    for (size_t i = 0; i < 2; i++)
    {
        circularBuffer_pop(&circBuff);
    }
    
    for (size_t i = 0; i < 2; i++)
    {
        circularBuffer_push(&circBuff, pushAgain[i]);
    }

    TEST_ASSERT_EQUAL_CHAR(expectedChar_0, circBuff.buff_origin[0]);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedArray, circBuff.buff_origin, 8);
}

// check if push the head back to the first node works correctly
void test_circularBuffer_push_head_back_to_first_node(void)
{
    const size_t N = 7;
    char pushString[8] = "abcdefg";
    char pushAgain = 'A';
    char* expectedHead = circBuff.buff_origin;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    for (size_t i = 0; i < N; i++)
    {
        circularBuffer_push(&circBuff, pushString[i]);
    }
    circularBuffer_pop(&circBuff);
    
    circularBuffer_push(&circBuff, pushAgain);

    TEST_ASSERT_EQUAL_HEX32(expectedHead, circBuff.head);
}

///
/// circularBuffer_pop
///

// pop empty buffer after init - check tail
void test_circularBuffer_pop_after_init_check_tail(void)
{
    char* expectedValue; 
    char* observedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    expectedValue = circBuff.tail;

    circularBuffer_pop(&circBuff);
    observedValue = circBuff.tail;
    
    TEST_ASSERT_EQUAL_HEX32(expectedValue, observedValue);
}

// pop empty buffer after init - check return value
void test_circularBuffer_pop_after_init_check_return_value(void)
{
    const char expectedValue = EMPTY_BUFFER; 
    char observedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    observedValue = circularBuffer_pop(&circBuff);
    
    TEST_ASSERT_EQUAL_CHAR(expectedValue, observedValue);
}

// pop buffer once with one item - check tail
void test_circularBuffer_pop_after_one_push_check_tail(void)
{
    char c = 'c';
    char* expectedValue; 
    char* observedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    expectedValue = circBuff.tail + 1;
    circularBuffer_push(&circBuff, c);
    circularBuffer_pop(&circBuff);
    observedValue = circBuff.tail;
    
    TEST_ASSERT_EQUAL_HEX32(expectedValue, observedValue);
}

// pop buffer once with one item - check return value
void test_circularBuffer_pop_after_one_push_check_return_value(void)
{
    char c = 'c';
    const char expectedValue = c; 
    char observedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    circularBuffer_push(&circBuff, c);
    observedValue = circularBuffer_pop(&circBuff);
    
    TEST_ASSERT_EQUAL_CHAR(expectedValue, observedValue);
}


// pop full buffer once BUFFER_LEN + 1 times
void test_circularBuffer_pop_full_buffer_tail(void)
{
    char str[BUFFER_LEN] = "abcdefg";

    char* max_tail;
    char* expectedValue; 
    char* observedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    for (size_t i = 0; i < BUFFER_LEN - 1; i++)
    {
        circularBuffer_push(&circBuff, str[i]);
    } 

    //printf("origin: 0x%x\n", circBuff.buff_origin);
    for (size_t i = 0; i < BUFFER_LEN-1; i++)
    {
        circularBuffer_pop(&circBuff);
        expectedValue = circBuff.buff_origin + (i+1);
        observedValue = circBuff.tail;
        //printf("%d: exp: 0x%x, obs: 0x%x\n", i, expectedValue, observedValue);

        TEST_ASSERT_EQUAL_HEX32(expectedValue, observedValue);
    } 
}

// pop full buffer once BUFFER_LEN + 1 times
void test_circularBuffer_pop_full_buffer_return_value(void)
{
    char str[BUFFER_LEN] = "abcdefg";

    char expectedValue; 
    char observedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    for (size_t i = 0; i < BUFFER_LEN - 1; i++)
    {
        circularBuffer_push(&circBuff, str[i]);
    } 

    for (size_t i = 0; i < BUFFER_LEN - 1; i++)
    {
        observedValue = circularBuffer_pop(&circBuff);
        expectedValue = str[i];

        //printf("%d: exp: %c, obs: %c\n", i, expectedValue, observedValue);

        TEST_ASSERT_EQUAL_CHAR(expectedValue, observedValue);
    } 
}

// check if push the head back to the first node works correctly
void test_circularBuffer_pop_tail_back_to_first_node(void)
{
    const size_t N = 7;
    char pushString[8] = "abcdefg";
    char* expectedTail = circBuff.buff_origin;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    for (size_t i = 0; i < N; i++)
    {
        circularBuffer_push(&circBuff, pushString[i]);
    }

    for (size_t i = 0; i < N; i++)
    {
        circularBuffer_pop(&circBuff);
    }

    for (size_t i = 0; i < N; i++)
    {
        circularBuffer_push(&circBuff, pushString[i]);
    }

    circularBuffer_pop(&circBuff);

    TEST_ASSERT_EQUAL_HEX32(expectedTail, circBuff.tail);
}

///
/// circularBuffer_len
///
// check len
void test_circularBuffer_len(void)
{
    uint16_t expectedValue = 0;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    TEST_ASSERT_EQUAL_UINT16(expectedValue, circularBuffer_len(circBuff));

    circularBuffer_push(&circBuff, 'a');
    expectedValue++;
    TEST_ASSERT_EQUAL_UINT16(expectedValue, circularBuffer_len(circBuff));

    circularBuffer_push(&circBuff, 'a');
    expectedValue++;
    circularBuffer_push(&circBuff, 'a');
    expectedValue++;
    TEST_ASSERT_EQUAL_UINT16(expectedValue, circularBuffer_len(circBuff));
}

// check len full
void test_circularBuffer_len_full(void)
{
    const size_t N = BUFFER_LEN - 1;
    uint16_t expectedValue = BUFFER_LEN - 1;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    for (size_t i = 0; i < N; i++)
    {
        circularBuffer_push(&circBuff, 'a');
    }
    
    TEST_ASSERT_EQUAL_UINT16(expectedValue, circularBuffer_len(circBuff));
}

// check len overflow
void test_circularBuffer_len_overflow(void)
{
    const size_t N = 100;
    uint16_t expectedValue = BUFFER_LEN - 1;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    for (size_t i = 0; i < N; i++)
    {
        circularBuffer_push(&circBuff, 'a');
    }
    
    TEST_ASSERT_EQUAL_UINT16(expectedValue, circularBuffer_len(circBuff));
}

// push trice pop one
void test_circularBuffer_len_push3_pop1(void)
{
    const size_t PUSH = 3;
    const size_t POP = 1;
    const uint16_t expectedValue = PUSH - POP;
    uint16_t observedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    for (size_t i = 0; i < PUSH; i++)
    {
        circularBuffer_push(&circBuff, 'a');
    }
    for (size_t i = 0; i < POP; i++)
    {
        circularBuffer_pop(&circBuff);
    }
    observedValue = circularBuffer_len(circBuff);
    
    TEST_ASSERT_EQUAL_UINT16(expectedValue, circularBuffer_len(circBuff));
}

// push trice pop four times
void test_circularBuffer_len_push3_pop4(void)
{
    const size_t PUSH = 3;
    const size_t POP = 4;
    const uint16_t expectedValue = 0;
    uint16_t observedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    for (size_t i = 0; i < PUSH; i++)
    {
        circularBuffer_push(&circBuff, 'a');
    }
    for (size_t i = 0; i < POP; i++)
    {
        circularBuffer_pop(&circBuff);
    }
    observedValue = circularBuffer_len(circBuff);
    
    TEST_ASSERT_EQUAL_UINT16(expectedValue, circularBuffer_len(circBuff));
}

// push to overflow, pop 100 times
void test_circularBuffer_len_pushOverflow_pop100(void)
{
    const size_t PUSH = BUFFER_LEN;
    const size_t POP = 100;
    const uint16_t expectedValue = 0;
    uint16_t observedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    for (size_t i = 0; i < PUSH; i++)
    {
        circularBuffer_push(&circBuff, 'a');
    }
    for (size_t i = 0; i < POP; i++)
    {
        circularBuffer_pop(&circBuff);
        //printf("i: %d   head: 0x%x   tail: 0x%x\n", i, circBuff.head, circBuff.tail);
    }
    observedValue = circularBuffer_len(circBuff);
    
    TEST_ASSERT_EQUAL_UINT16(expectedValue, circularBuffer_len(circBuff));
}

///
/// circularBuffer_clean
///
void test_circularBuffer_clean_head(void)
{
    const size_t PUSH = 4;
    const size_t POP = 3;
    char* expectedValue = buffer;
    char* observedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    for (size_t i = 0; i < PUSH; i++)
    {
        circularBuffer_push(&circBuff, 'c');
    }
    for (size_t i = 0; i < POP; i++)
    {
        circularBuffer_pop(&circBuff);
    }    

    observedValue = circBuff.head;
    TEST_ASSERT_EQUAL_HEX32(buffer + PUSH, observedValue);
    
    circularBuffer_clean(&circBuff);
    observedValue = circBuff.head;
    TEST_ASSERT_EQUAL_HEX32(expectedValue, observedValue);
}

void test_circularBuffer_clean_tail(void)
{
    const size_t PUSH = 4;
    const size_t POP = 3;
    char* expectedValue = buffer;
    char* observedValue;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);
    for (size_t i = 0; i < PUSH; i++)
    {
        circularBuffer_push(&circBuff, 'c');
    }
    for (size_t i = 0; i < POP; i++)
    {
        circularBuffer_pop(&circBuff);
    } 

    observedValue = circBuff.tail;
    TEST_ASSERT_EQUAL_HEX32(buffer + POP, observedValue);   
    
    circularBuffer_clean(&circBuff);
    observedValue = circBuff.tail;
    TEST_ASSERT_EQUAL_HEX32(expectedValue, observedValue);
}

// check if push string saves correctly to string
void test_circularBuffer_push_string(void)
{
    int N = 7;
    const char inputString[8] = "abcdefg";
    char* expectedString = inputString;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    circularBuffer_pushString(&circBuff, inputString);

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedString, circBuff.buff_origin, N);
}

// check if push string returns proper pointer if it should be null
void test_circularBuffer_push_string_ret_val_null(void)
{
    int N = 8;
    const char inputString[9] = "abcdefgh";
    char* expected_retVal = (char*)NULL;
    char* observed_retVal;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    observed_retVal = circularBuffer_pushString(&circBuff, inputString);

    TEST_ASSERT_EQUAL_HEX32(expected_retVal, observed_retVal);
}

// check if push string returns proper pointer if it should not be null
void test_circularBuffer_push_string_ret_val_not_null(void)
{
    int N = 7;
    const char inputString[8] = "abcdefg";
    char* expected_retVal = circBuff.buff_origin + N;
    char* observed_retVal;

    circularBuffer_init(&circBuff, buffer, BUFFER_LEN);

    observed_retVal = circularBuffer_pushString(&circBuff, inputString);

    TEST_ASSERT_EQUAL_HEX32(expected_retVal, observed_retVal);
}

void test_ignore(void)
{
    TEST_IGNORE();
}


int main(void)
{
    UNITY_BEGIN();
    
    // first test
    RUN_TEST(test_firstFailingTest);
    // init
    RUN_TEST(test_circularBuffer_init_return);
    RUN_TEST(test_circularBuffer_init_charBuffer);
    RUN_TEST(test_circularBuffer_init_head);
    RUN_TEST(test_circularBuffer_init_tail);
    RUN_TEST(test_circularBuffer_init_size);
    RUN_TEST(test_circularBuffer_init_len);
    // max size
    RUN_TEST(test_circularBuffer_max_size);
    // push
    RUN_TEST(test_circularBuffer_push_one);
    RUN_TEST(test_circularBuffer_push_one_head);
    RUN_TEST(test_circularBuffer_push_one_tail);
    RUN_TEST(test_circularBuffer_push_three);
    RUN_TEST(test_circularBuffer_push_three_head);
    RUN_TEST(test_circularBuffer_push_overflow);
    RUN_TEST(test_circularBuffer_push_overflow_head);
    RUN_TEST(test_circularBuffer_push_overflow_memory);
    RUN_TEST(test_circularBuffer_push_again);
    RUN_TEST(test_circularBuffer_push_head_back_to_first_node);
    // pop
    RUN_TEST(test_circularBuffer_pop_after_init_check_tail);
    RUN_TEST(test_circularBuffer_pop_after_init_check_return_value);
    RUN_TEST(test_circularBuffer_pop_after_one_push_check_tail);
    RUN_TEST(test_circularBuffer_pop_after_one_push_check_return_value);
    RUN_TEST(test_circularBuffer_pop_full_buffer_tail);
    RUN_TEST(test_circularBuffer_pop_full_buffer_return_value);
    RUN_TEST(test_circularBuffer_pop_tail_back_to_first_node);
    // len
    RUN_TEST(test_circularBuffer_len);
    RUN_TEST(test_circularBuffer_len_full);
    RUN_TEST(test_circularBuffer_len_overflow);
    RUN_TEST(test_circularBuffer_len_push3_pop1);
    RUN_TEST(test_circularBuffer_len_push3_pop4);
    RUN_TEST(test_circularBuffer_len_pushOverflow_pop100);
    // clean
    RUN_TEST(test_circularBuffer_clean_head);
    RUN_TEST(test_circularBuffer_clean_tail);
    // push string
    RUN_TEST(test_circularBuffer_push_string);
    RUN_TEST(test_circularBuffer_push_string_ret_val_null); 
    RUN_TEST(test_circularBuffer_push_string_ret_val_not_null);

    RUN_TEST(test_ignore);

    

    return UNITY_END();
}
