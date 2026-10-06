#ifndef REQUEST_HTTP_H
#define REQUEST_HTTP_H
#include <stddef.h>
typedef enum {
    METHOD_INVALID,
    GET
}method_t;

typedef struct {
    method_t method;
    char* url;
}http_request_t;

const char* method_to_str(method_t method);
void REQUEST_init(http_request_t* request);
void REQUEST_free_all(http_request_t* request);
#endif