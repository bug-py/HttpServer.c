#include "http_protocol/request.h"
#include "struct_data/safe_alloc.h"
#include <stdlib.h>
const char* method_to_str(method_t method){
    switch(method){
        case GET: return "GET";
        default : return NULL;
    }
}
http_request_t* REQUEST_create(void){
    http_request_t* request=xalloc(sizeof(http_request_t),1,NULL);
    REQUEST_init(request);
    return request;
}
void REQUEST_init(http_request_t* request){
    request->method=METHOD_INVALID;
    request->url=NULL;
}

void REQUEST_free_all(http_request_t* request){
    if(request->url) free(request->url);
}
void REQUEST_destroy(http_request_t* request){
    if(request!=NULL){
        REQUEST_free_all(request);
        free(request);
    }
   
}