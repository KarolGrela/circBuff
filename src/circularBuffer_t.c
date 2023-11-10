#include "circularBuffer_t.h"

// LOCAL DEFINITIONS

// STATIC FUNCTIONS
static char* getNextHead(struct circularBuffer_t circ_buff)
{
    char* buffer_border = circ_buff.buff_origin + circ_buff.size - 1;
    
    if(circ_buff.head == (unsigned char*)buffer_border)
    {
        if(circ_buff.tail == circ_buff.buff_origin)
        {
            //
            return NULL;
        }
        else
        {
            //
            return circ_buff.buff_origin;
        }
    }
    else if(circ_buff.head + 1 == circ_buff.tail)
    {
        return NULL;
    }
    else
    {
        return circ_buff.head + 1;
    }
}

static char* getNextTail(struct circularBuffer_t circ_buff)
{
    char* buffer_border = circ_buff.buff_origin + circ_buff.size - 1;
    if(circ_buff.tail == circ_buff.head)
    {
        return NULL;
    }
    else if(circ_buff.tail == (unsigned char*)buffer_border)
    {
        if(circ_buff.head == circ_buff.buff_origin)
        {
            //
            return NULL;
        }
        else
        {
            //
            return circ_buff.buff_origin;
        }
    }
    else
    {
        return circ_buff.tail + 1;
    }
}

//
// GLOBAL FUNCTIONS
//

struct circularBuffer_t* circularBuffer_init(struct circularBuffer_t* circ_buff, unsigned char* str_buff, uint16_t str_buff_len)
{
    circ_buff->buff_origin = str_buff;
    
    circ_buff->head = circ_buff->buff_origin;
    circ_buff->tail = circ_buff->buff_origin;
    
    circ_buff->size = str_buff_len;

    return circ_buff;
}

void circularBuffer_clean(struct circularBuffer_t* circ_buff)
{
    circ_buff->head = circ_buff->buff_origin;
    circ_buff->tail = circ_buff->buff_origin;
}

void circularBuffer_push(struct circularBuffer_t* circ_buff, unsigned char _input)
{
    char* nextHead = getNextHead(*circ_buff);

    if(nextHead != NULL)
    {
        *(circ_buff->head) = _input;
        circ_buff->head = nextHead;   
    }   
}

char circularBuffer_pop(struct circularBuffer_t* circ_buff)
{
    char _output = *(circ_buff->tail);
    char* next_tail = getNextTail(*circ_buff);
    if(next_tail != NULL)
    {
        circ_buff->tail = next_tail;
        return _output;
    }
    else
    {
        return EMPTY_BUFFER;    
    }
}

uint16_t circularBuffer_len(struct circularBuffer_t circ_buff)
{
    if(getNextHead(circ_buff) == NULL)
    {
        return circ_buff.size - 1;
    }
    else
    {
        if(circ_buff.head == circ_buff.tail)
        {
            return 0;
        }
        else if(circ_buff.head > circ_buff.tail)
        {
            return circ_buff.head - circ_buff.tail;
        }
        else if(circ_buff.head < circ_buff.tail)
        {
            return circ_buff.size - (circ_buff.tail - circ_buff.head);
        }
    }

    return -1;
}

uint16_t circularBuffer_max_size(struct circularBuffer_t circ_buff)
{
    return circ_buff.size;
}

char* circularBuffer_pushString(struct circularBuffer_t* circ_buff, const char* input_string)
{
    char* head = circ_buff->head;
    char* prev_head = circ_buff->head;

    for (size_t i = 0; i < strlen(input_string); i++)
    {
        circularBuffer_push(circ_buff, input_string[i]);
        
        prev_head = head;
        head = circ_buff->head;
    }

    if(prev_head == head)
    {
        return NULL;
    }
    else
    {
        return head;
    }
    
}