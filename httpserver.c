#include <stdio.h>
#include <stdlib.h>
#include "api/server.h"
#include "struct_data/safe_alloc.h"

#define IP "127.0.0.1"
#define PORT 3000

http_response_t* directory_mapping(char* ip,in_port_t port,http_request_t* req){
    (void) ip;(void)port;(void)req;
    http_response_t* res=RESPONSE_create();
    res->status_code=200;
    return res;

}

int main(){
    printf("http://%s:%i\n",IP,PORT);
    switch(HTTP_SERVER_loop(IP,PORT,10,&directory_mapping)){
        case SERVER_SYS_ERR:
            perror("sys server");
            return -1;
        case SERVER_IP_ERR:
            fprintf(stderr,"ip format error\n");
            return -1;
    }
    return 0;
}