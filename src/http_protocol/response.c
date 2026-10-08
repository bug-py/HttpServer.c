#include "http_protocol/response.h"
#include "http_protocol/http.h"
#include <stdio.h>
#include <stdlib.h>
int is_valid_status_code(status_code_t status_code){
    return status_code>=100 && status_code<=599;
}
const char* status_code_to_str(status_code_t status_code){
    switch(status_code){
        case 200: return "OK";
        case 404: return "Not Found";
    }
    // generic reason phrase
    if(status_code>=100 && status_code<=199) return "Info";
    if(status_code>=200 && status_code<=299) return "Success";
    if(status_code>=300 && status_code<=399) return "Redirection";
    if(status_code>=400 && status_code<=499) return "Client Error";
    if(status_code>=500 && status_code<=599) return "Server Error";
    // Not standard
    return NULL;
}
void RESPONSE_init(http_response_t* response){
    response->status_code=-1;
    response->content_length=0;
    response->reason_phrase=NULL;
    response->content_type=NULL;
    response->content=NULL;
}
int RESPONSE_build(http_response_t* response,buffer_t* buffer){
    if(!is_valid_status_code(response->status_code)) return -1;
    if(response->content_length>0 && response->content==NULL) return -1;
    if(BUFFER_append(buffer,HTTP_VERSION,HTTP_VERSION_LEN)<0) return -1;
    char status_code_and_spaces[HTTP_VERSION_LEN+2];
    snprintf(status_code_and_spaces,HTTP_VERSION_LEN+2," %i ",response->status_code);
    return 0;
    
}
void RESPONSE_free_all(http_response_t* response){
    if(response->content && response->content_length>0) free(response->content);
}
void RESPONSE_destroy(http_response_t* response){
    if(response!=NULL){
        RESPONSE_free_all(response);
        free(response);
    }
   
}