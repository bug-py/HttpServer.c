#include "http_protocol/request.h"
#include <stdlib.h>
const char* method_to_str(method_t method){
    switch(method){
        case GET: return "GET";
        default : return NULL;
    }
}
void REQUEST_init(http_request_t* request){
    request->method=METHOD_INVALID;
    request->url=NULL;
}
void REQUEST_free_all(http_request_t* request){
    if(request->url) free(request->url);
}