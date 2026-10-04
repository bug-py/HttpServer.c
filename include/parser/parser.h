#ifndef PARSER_H
#define PARSER_H
#include <stddef.h>
#include "http_protocol/request.h"
typedef enum{
    PARSING_OVERFLOW=-2,
    PARSING_ERR=-1,
    PARSING_SUCCES=0,
    PARSING_WAIT=1
}parser_result_t;

typedef struct{
    char* buffer;
    size_t capacity;
    size_t length;
    size_t limit_length;
    http_request_t* current;

}parser_context_t;

parser_result_t parser_feed(parser_context_t* ctx,char* stream,size_t len);
#endif