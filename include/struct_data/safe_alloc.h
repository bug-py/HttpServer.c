#ifndef SAFE_ALLOC_H
#define SAFE_ALLOC_H
#include <stddef.h>
#include <stdlib.h>
void* xalloc(size_t size,size_t element,void* old_ptr);
#endif