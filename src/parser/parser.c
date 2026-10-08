#include "parser/parser.h"
#include "http_protocol/http.h"
#include "struct_data/safe_alloc.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#define INIT_CAPACITY_BUFFER 1024
void PARSER_init(parser_context_t* ctx,size_t max_length){
    ctx->bytes_processed=0;
    ctx->state=CTX_PARSING_START_LINE;
    ctx->request=xalloc(sizeof(http_request_t),1,NULL);
    REQUEST_init(ctx->request);
    BUFFER_init(&(ctx->buffer),INIT_CAPACITY_BUFFER,max_length);
}
char* find_char(char* current,size_t len,char character){
    for(size_t i=0;i<len;i++){
        if(current[i]==character) return &(current[i]);
    }
    return NULL;
}
char* search_CRLF(char* current,size_t len){
    for (size_t i=1;i<len;i++){
        if(current[i]=='\n' && current[i-1]=='\r') return &(current[i-1]);   
    }
    return NULL;
}
bool startwith(const char* current,size_t len_current,const char* cmp,size_t len_cmp,char** next,size_t* remaining_len){
    if(len_cmp>len_current) return false;
    for (size_t i=0;i<len_cmp;i++){
        if(current[i] != cmp[i])  return false;
    }
    *next=(char*)(uintptr_t)current+(uintptr_t)len_cmp;
    *remaining_len=len_current-len_cmp;
    return true;
}
method_t extract_method(const char* current,size_t len,char** next,size_t* remaining_len){
        if(startwith(current,len,method_to_str(GET),strlen(method_to_str(GET)),next,remaining_len)){
            return GET;
        }else{
            return METHOD_INVALID;
        }
}
char* extract_url(char* current,size_t len,char** next,size_t* remaining_len){
    char* space=find_char(current,len,' ');
    if(space==NULL) return NULL;
    size_t len_url=(uintptr_t)space-(uintptr_t)current;
    char* url=xalloc(sizeof(char),len_url+1,NULL);
    memcpy(url,current,len_url);
    url[len_url]='\0';
    *remaining_len=len-len_url;
    *next=space;
    return url;
    
}
parser_result_t PARSER_feed(parser_context_t* ctx,char* stream,size_t len){
   
    if(BUFFER_append(&(ctx->buffer),stream,len)<0){
        ctx->state=CTX_PARSING_OVERFLOW;
        return  PARSING_OVERFLOW;
    }
    if(ctx->state==CTX_PARSING_FINISH) return PARSING_SUCCES;
    if(ctx->state==CTX_PARSING_OVERFLOW) return PARSING_OVERFLOW;
    if(ctx->state==CTX_PARSING_ERR) return PARSING_ERR;
    if(ctx->state==CTX_PARSING_START_LINE){
        // Check CRFL
        char* CRLF=search_CRLF(ctx->buffer.data,ctx->buffer.length);
        if(CRLF==NULL) return PARSING_NEED_MORE_DATA;
       
        size_t start_line_len=(uintptr_t)CRLF-(uintptr_t)ctx->buffer.data;
        size_t current_len;
        char* space;

        // extract method and fill request
        method_t method=extract_method(ctx->buffer.data,start_line_len,&space,&current_len);
        if(method==METHOD_INVALID) goto error;
        ctx->request->method=method;

        //  eat space
        if(current_len==0 || *space!=' ')  goto error;
        
        //  extract url and fill request
        char* url=extract_url(space+1,current_len-1,&space,&current_len);
        if(url==NULL) goto error;
        ctx->request->url=url;

        //  eat space
        if(current_len==0 || *space!=' ')  goto error;

        // check version
        if(!startwith(space+1,current_len-1,HTTP_VERSION,HTTP_VERSION_LEN,&space,&current_len)){
            goto error;
        }
    
        if(current_len!=0) goto error;
        ctx->bytes_processed=start_line_len+HTTP_CRLF_LEN;
        ctx->state=CTX_PARSING_HEADERS;
    }
    if(ctx->state==CTX_PARSING_HEADERS){
        // loop for catch each header in the buffer
        while(1){
            char* start_header=ctx->buffer.data+ctx->bytes_processed;
            char* CRLF=search_CRLF(start_header,ctx->buffer.length-ctx->bytes_processed);
            if(CRLF==NULL) return PARSING_NEED_MORE_DATA;
            size_t len_header=(char*)CRLF-(char*)start_header;
            // ignore headers for this version
            ctx->bytes_processed+=len_header+HTTP_CRLF_LEN;
            // there are nothing between : '\r\n\r\n'
            // end of headers 
            if(len_header==0){
                ctx->state=CTX_PARSING_BODY;
                break;
            }

        }
    }
    if(ctx->state==CTX_PARSING_BODY){
        // ignore body for this version
        ctx->state=CTX_PARSING_FINISH;
    }
    return  PARSING_SUCCES;
    error: 
        ctx->state=CTX_PARSING_ERR;
        return PARSING_ERR;  
}
http_request_t* PARSER_get_request(parser_context_t* ctx){
    if(ctx->state==CTX_PARSING_FINISH){
        http_request_t* request=ctx->request;
        ctx->request=NULL;
        return request;
    }
    return NULL;

}
void PARSER_reset(parser_context_t* ctx){
    REQUEST_destroy(ctx->request);
    ctx->request=xalloc(sizeof(http_request_t),1,NULL);
    REQUEST_init(ctx->request);
    ctx->bytes_processed=0;
    ctx->state=CTX_PARSING_START_LINE;
    BUFFER_clear(&(ctx->buffer));
}
void PARSER_free(parser_context_t* ctx){
    REQUEST_destroy(ctx->request);
    BUFFER_free(&(ctx->buffer));
}