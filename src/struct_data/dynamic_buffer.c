#include "struct_data/dynamic_buffer.h"
#include <stdlib.h>
#define DEFAULT_INIT_CAPACITY sizeof(void*)
void BUFFER_init(buffer_t* buffer,size_t init_capacity,size_t max_length){
    if(init_capacity==0) init_capacity=DEFAULT_INIT_CAPACITY;
    buffer->capacity=init_capacity;
    buffer->max_length=max_length;
    buffer->length=0;
    buffer->data=malloc(init_capacity);
}