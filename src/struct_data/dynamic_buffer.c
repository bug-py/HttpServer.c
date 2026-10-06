#include "struct_data/dynamic_buffer.h"
#include "struct_data/safe_alloc.h"
#include <string.h>
#define DEFAULT_INIT_CAPACITY sizeof(void*)
void BUFFER_init(buffer_t* buffer,size_t init_capacity,size_t max_length){
    if(init_capacity==0) init_capacity=DEFAULT_INIT_CAPACITY;
    buffer->capacity=init_capacity;
    buffer->max_length=max_length;
    buffer->length=0;
    buffer->data=xalloc(sizeof(char),init_capacity,NULL);
}
int BUFFER_append(buffer_t* buffer,void* stream,size_t len){
    size_t new_length=buffer->length+len;
    if(new_length>buffer->max_length) return -1;
    if(new_length>buffer->capacity){
        size_t new_capacity=new_length*2;
        buffer->capacity=new_capacity;
        buffer->data=xalloc(sizeof(char),new_capacity,buffer->data);
    }
    memcpy(buffer->data+buffer->length,stream,len);
    buffer->length=new_length;
    return 0;
    
}
void BUFFER_clear(buffer_t* buffer){
    buffer->length=0;
}
void BUFFER_free(buffer_t* buffer){
    free(buffer->data);
}