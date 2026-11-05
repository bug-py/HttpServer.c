#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api/server.h"
http_response_t* handle(char *ip,in_port_t port,http_request_t* request){
    (void)ip;
    (void)port;
    http_response_t* response=malloc(sizeof(http_response_t));
    RESPONSE_init(response);
    response->content=strdup(request->url);
    response->content_length=strlen(response->content);
    response->status_code=200;
    return response;
}
int main(){
    int result=HTTP_SERVER_loop("127.0.0.1",3000,10,&handle);
    switch(result){
        case SERVER_SYS_ERR:
            perror("server");
            return -1;
        case SERVER_IP_ERR:
            fprintf(stderr,"ip format error\n");
            return -1;
    }
    return 0;
}