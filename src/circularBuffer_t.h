#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define EMPTY_BUFFER -1

struct circularBuffer_t 
{
    unsigned char* head;
    unsigned char* tail;

    uint16_t size;

    unsigned char* buff_origin; // address of first byte in buffer in memory
};

struct circularBuffer_t* circularBuffer_init(struct circularBuffer_t*, unsigned char*, uint16_t);

void circularBuffer_clean(struct circularBuffer_t*);

void circularBuffer_push(struct circularBuffer_t*, unsigned char);

char circularBuffer_pop(struct circularBuffer_t*);

uint16_t circularBuffer_len(struct circularBuffer_t);

uint16_t circularBuffer_max_size(struct circularBuffer_t);

char* circularBuffer_pushString(struct circularBuffer_t*, const char*);
