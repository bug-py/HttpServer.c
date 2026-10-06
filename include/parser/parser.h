#ifndef PARSER_H
#define PARSER_H
#include <stddef.h>
#include "http_protocol/request.h"
#include "struct_data/dynamic_buffer.h"
typedef enum{
    PARSING_OVERFLOW=-2,
    PARSING_ERR=-1,
    PARSING_SUCCES=0,
    PARSING_NEED_MORE_DATA=1
}parser_result_t;
typedef enum{
    CTX_PARSING_OVERFLOW=-2,
    CTX_PARSING_ERR=-1,
    CTX_PARSING_START_LINE=0,
    CTX_PARSING_HEADERS=1,
    CTX_PARSING_BODY=2,
    CTX_PARSING_FINISH=3
}parser_state_t;
typedef struct{
    buffer_t buffer;
    size_t bytes_processed;
    parser_state_t state;
    http_request_t* request;

}parser_context_t;
void PARSER_init(parser_context_t* ctx,size_t max_length);
parser_result_t PARSER_feed(parser_context_t* ctx,char* stream,size_t len);
#endif