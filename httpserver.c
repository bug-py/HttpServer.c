#include <stdio.h>
#include <stdlib.h>
#include "api/server.h"
#include "struct_data/safe_alloc.h"

#define IP "127.0.0.1"
#define PORT 3000
char* read_file(char* path,size_t* ptr_len){
    FILE* file=fopen(path,"rb");
    char* content=NULL;
    if(file==NULL) return NULL;

    if(fseek(file,0,SEEK_END)<0) goto clean;
    off_t len=ftello(file);
    if(len<0) goto clean;

    rewind(file);

    content=xalloc(sizeof(char),(size_t)len,NULL);
    size_t bloc=fread(content,(size_t)len,1,file);
    if (bloc!=1) goto clean;

    *ptr_len=(size_t)len;
    fclose(file);
    return content;

    clean:
        fclose(file);
        free(content);
        return NULL;  
   
}
http_response_t* directory_mapping(char* ip,in_port_t port,http_request_t* req){
    (void) ip;(void)port;(void)req;
    http_response_t* res=RESPONSE_create();
    res->status_code=200;
    return res;

}

int main(){
    size_t len;
    char* file=read_file("static/index.html",&len);
    printf("%p\n",file);
    for(size_t i=0;i<len;i++){
        putchar(file[i]);
    }
    putchar('\n');
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