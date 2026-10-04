#include "http_protocol/response.h"

const char* status_code_to_str(status_code_t status_code){
    switch(status_code){
        case 200: return "OK";
        case 404: return "Not Found";
        default : return NULL;
    }
}