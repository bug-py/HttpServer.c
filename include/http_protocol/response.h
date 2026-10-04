#ifndef RESPONSE_HTTP_H
#define RESPONSE_HTTP_H
#include <stdint.h>
#include <stddef.h>
typedef uint16_t status_code_t;

typedef struct {
    status_code_t status_code;
    size_t content_length;
    char* content;
}http_response_t;

const char* status_code_to_str(status_code_t status_code);
#endif