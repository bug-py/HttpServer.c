#ifndef DYNAMIC_BUFFER_H
#define DYNAMIC_BUFFER_H
#include <stddef.h>
typedef struct{
    char* data;
    size_t length;
    size_t max_length;
    size_t capacity;
}buffer_t;
void BUFFER_init(buffer_t* buffer,size_t init_capacity,size_t max_length);
#endif