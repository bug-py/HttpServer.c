#include "http_protocol/request.h"

const char* method_to_str(method_t method){
    switch(method){
        case GET: return "GET";
        default : return NULL;
    }
}