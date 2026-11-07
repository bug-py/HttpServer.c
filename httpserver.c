#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api/server.h"
#include "struct_data/safe_alloc.h"

#define IP "127.0.0.1"
#define PORT 3000

#define DIR_STATIC "static"
#define ROOT_PATH_FILE "/index.html"

char* read_file(char* path,size_t* ptr_len){
    FILE* file=fopen(path,"rb");
    char* content=NULL;
    if(file==NULL) return NULL;
    // get length file
    if(fseek(file,0,SEEK_END)<0) goto clean;
    off_t len=ftello(file);
    if(len<0) goto clean;
    rewind(file);
    // read all file
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
char* ext_to_MIME(char* file_path){
    char* ext=strrchr(file_path,'.');
    if(ext==NULL) return "text/plain";
    if(strcmp(ext,".html")==0) return "text/html";
    if(strcmp(ext,".css")==0) return "text/css";
    if(strcmp(ext,".js")==0) return "text/javascript";
    if(strcmp(ext,".png")==0) return "image/png";
    if(strcmp(ext,".jpeg")==0) return "image/jpeg";
    if(strcmp(ext,".jpg")==0) return "image/jpg";
    if(strcmp(ext,".webp")==0) return "image/webp";
    if(strcmp(ext,".ttf")==0) return "font/ttf";
    
    return "text/plain";
}
char* strjoin(char* first,char* second){
    char* result=xalloc(sizeof(char),strlen(first)+strlen(second)+1,NULL);
    strcpy(result,first);
    strcat(result,second);
    return result;
}
http_response_t* directory_mapping(char* ip,in_port_t port,http_request_t* req){
    // if the path is /, serve the default file
    char* path= strcmp(req->url,"/")==0 ? strjoin(DIR_STATIC,ROOT_PATH_FILE) :strjoin(DIR_STATIC,req->url);
    size_t len;
    char* content=read_file(path,&len);
    printf("%s:%i  => %s \n",ip,port,path);
    http_response_t* res=RESPONSE_create();
    if(content==NULL){
        // file not found 
        char* error=strjoin("404 not found : ",req->url);
        res->status_code=404;
        res->content=error;
        res->content_length=strlen(error);
        res->content_type=strdup("text/plain");
        free(content);
    }else{
        // file found
        res->status_code=200;
        res->content=content;
        res->content_length=len;
        res->content_type=strdup(ext_to_MIME(path));
    }
    free(path);
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