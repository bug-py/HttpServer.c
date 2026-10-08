#ifndef RESPONSE_HTTP_H
#define RESPONSE_HTTP_H
#include <stdint.h>
#include <stddef.h>
#include "struct_data/dynamic_buffer.h"
#define STATUS_CODE_DIGIT 3
typedef int status_code_t;

typedef struct {
    status_code_t status_code;
    size_t content_length;
    char* content_type;
    char* reason_phrase;
    char* content;
}http_response_t;

const char* status_code_to_str(status_code_t status_code);
void RESPONSE_init(http_response_t* response);
int RESPONSE_build(http_response_t* response,buffer_t* buffer);
void RESPONSE_free_all(http_response_t* response);
void RESPONSE_destroy(http_response_t* response);
#endif