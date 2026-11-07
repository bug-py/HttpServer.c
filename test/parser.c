#include "parser/parser.h"
#include <stdio.h>
#include <string.h>
void print_result(parser_result_t result){
    switch(result){
        case PARSING_NEED_MORE_DATA:
            printf("NEED MORE DATA\n");
            break;
        case PARSING_SUCCES:
            printf("PARSING END\n");
            break;
        case PARSING_ERR:
            printf("ERROR\n");
            break;
        case PARSING_OVERFLOW:
            printf("OVERFLOW\n");
            break;
        default:
            printf("INVALID\n");
    }
}
void print_request(http_request_t* request){
    printf("METHOD : %s | ",method_to_str(request->method));
    printf("URL : %s\n",request->url);
}
int main(){
    parser_context_t ctx;
    PARSER_init(&ctx,1024);
    char* stream="GET /index.html HTTP/1.1\r";
    parser_result_t result=PARSER_feed(&ctx,stream,strlen(stream));
    print_result(result);
    stream="\nContent-Length: 0\r\n\r\n";
    result=PARSER_feed(&ctx,stream,strlen(stream));
    print_result(result);
    http_request_t* request=PARSER_get_request(&ctx);
    print_request(request);
    PARSER_reset(&ctx);
    stream="GET /home/login?name=";
    result=PARSER_feed(&ctx,stream,strlen(stream));
    print_result(result);
    stream="user4554 HTTP/1.1\r\n";
    result=PARSER_feed(&ctx,stream,strlen(stream));
    print_result(result);
    stream="\r\n";
    result=PARSER_feed(&ctx,stream,strlen(stream));
    print_result(result);
    request=PARSER_get_request(&ctx);
    print_request(request);
}