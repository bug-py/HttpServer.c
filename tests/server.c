#include <stdio.h>
#include <unistd.h>
#include "api/server.h"
int main(){
    int result=HTTP_SERVER_loop("127.0.0.1",3000,10,NULL);
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