#ifndef REQUEST_HTTP_H
#define REQUEST_HTTP_H
#include <stddef.h>
typedef enum {
    GET
}method_t;

typedef struct {
    method_t method;
    char* url;
}http_request_t;

const char* method_to_str(method_t method);
#endif