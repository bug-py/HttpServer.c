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
int BUFFER_append(buffer_t* buffer,void* stream,size_t len);
void BUFFER_clear(buffer_t* buffer);
void BUFFER_free(buffer_t* buffer);
#endif