#include "http_protocol/response.h"
#include <stdio.h>
#include <string.h>
int main(){
    http_response_t response;
    RESPONSE_init(&response);
    response.status_code=200;
    response.reason_phrase="CUSTOM";
    response.content_type="test/html";
    response.content="<h1>HELLO</h1>";
    response.content_length=strlen(response.content);
    buffer_t buffer;
    BUFFER_init(&buffer,1024,0);
    int ret=RESPONSE_build(&response,&buffer);
    if(ret<0){
        fprintf(stderr,"ERROR BUILD\n");
        return 1;
    }
    for(size_t i=0;i<buffer.length;i++){
        putchar(buffer.data[i]);
    }
    putchar('\n');
    BUFFER_free(&buffer);
}