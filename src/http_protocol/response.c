#include "http_protocol/response.h"
#include "http_protocol/http.h"
#include <stdio.h>
#include <stdlib.h>
#define LEN_STR_STATUS_CODE (STATUS_CODE_DIGIT+3)
char* status_code_to_str(status_code_t status_code){
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
int is_valid_status_code(status_code_t status_code){
    return status_code>=100 && status_code<=599;
}

int RESPONSE_build(http_response_t* response,buffer_t* buffer){
    // version HTTP
    if(BUFFER_append_str(buffer,HTTP_VERSION)<0) return -1;

    // status code and 2 spaces   ex : " 200 "
    char str_status_code[LEN_STR_STATUS_CODE];
    if(!is_valid_status_code(response->status_code)) return -1;
    int ret=snprintf(str_status_code,LEN_STR_STATUS_CODE ," %i ",response->status_code);
    if(ret+1!=LEN_STR_STATUS_CODE) return -1;
    if(BUFFER_append_str(buffer,str_status_code)<0) return -1;

    // reason phrase
    char* reason_phrase=response->reason_phrase ? response->reason_phrase: status_code_to_str(response->status_code);
    if(BUFFER_append_str(buffer,reason_phrase)<0) return -1;

    // CRLF
    if(BUFFER_append_str(buffer,HTTP_CRLF)<0) return -1;


    // Content-Length header
    if(response->content_length>0){
        if(BUFFER_append_str(buffer,"Content-Length:")<0) return -1;
        // Number size_t => str
        char str_content_length[LEN_STR_MAX_LENGTH];
        int len_write=snprintf(str_content_length,LEN_STR_MAX_LENGTH,"%lu",response->content_length);
        if(len_write<0 || len_write+1>LEN_STR_MAX_LENGTH) return -1;
        if(BUFFER_append_str(buffer,str_content_length)<0) return -1;
        // CRLF
        if(BUFFER_append_str(buffer,HTTP_CRLF)<0) return -1;
    }
    // Content-Type header
    if(response->content_type){
        if(BUFFER_append_str(buffer,"Content-Type:")<0) return -1;
        if(BUFFER_append_str(buffer,response->content_type)) return -1;
        // CRLF
        if(BUFFER_append_str(buffer,HTTP_CRLF)<0) return -1;
    }

    // CRLF
    if(BUFFER_append_str(buffer,HTTP_CRLF)<0) return -1;

    // Content
    if(response->content_length>0){
        if(response->content==NULL) return -1;
        if(BUFFER_append(buffer,response->content,response->content_length)<0) return -1;
    }
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