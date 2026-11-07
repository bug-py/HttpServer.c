#include "struct_data/dynamic_buffer.h"
#include <stdio.h>
int main(){
    buffer_t buffer;
    BUFFER_init(&buffer,0,8);
    char stream[4]={'a','b','c','d'};
    BUFFER_append(&buffer,stream,4);
    printf("%c\n",buffer.data[0]);
    BUFFER_append(&buffer,stream,4);
    printf("%c\n",buffer.data[4]);
    int err=BUFFER_append(&buffer,stream,4);
    if(err<0){
        printf("max limit\n");
    }
    BUFFER_free(&buffer);
    return 0;
}