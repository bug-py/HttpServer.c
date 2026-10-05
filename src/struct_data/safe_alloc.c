#include "struct_data/safe_alloc.h"
#include <stdio.h>
void* xalloc(size_t size,size_t element,void* old_ptr){
    void* new_ptr=realloc(old_ptr,size*element);
    if(new_ptr==NULL){
        fprintf(stderr,"alloc failed\n");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}