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
int main(){
    parser_context_t ctx;
    PARSER_init(&ctx,1024);
    char* stream="GET /index.html HTTP/1.1\r\nContent-Length: 0\r\n\r\n";
    parser_result_t result=PARSER_feed(&ctx,stream,strlen(stream));
    print_result(result);
    printf("%s\n",ctx.request->url);

}