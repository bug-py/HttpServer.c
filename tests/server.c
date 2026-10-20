#include <stdio.h>
#include <unistd.h>
#include "api/server.h"
int main(){
    int result=CreateServer("127.0.0.1",3000,10);
    switch(result){
        case CREATE_SERVER_SYS_ERR:
            perror("create server");
            return -1;
        case CREATE_SERVER_IP_ERR:
            fprintf(stderr,"ip format error\n");
            return -1;
    }
    printf("serverfd : %i\n",result);
    close(result);
    return 0;
}