#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#define LENGTH_MAX_NUMBER_DIGIT 15
#define PORT 1080
char* request_start_line="HTTP/1.1 200 OK\r\n";
char* request_headers="Server : Httpserver.c\r\nContent-Type: text/html\r\nContent-Length : ";
char* request_marker_body="\r\n\r\n";

char* read_file(char* name_file,size_t* size_ptr){
    FILE* file=fopen(name_file,"rb");
    if(file==NULL) return NULL;

    fseek(file,0,SEEK_END);
    long size=ftell(file);
    if(size<0) goto error;
    rewind(file);
    char* buffer=malloc((size_t)size);
    if(buffer==NULL) goto error;

    *size_ptr=fread(buffer,sizeof(char),(size_t)size,file);
    
    fclose(file);
    return buffer;
    error:
     fclose(file);
     return NULL;
}
int send_data(int clientfd,char* buffer,size_t size){
    while(size!=0){
        ssize_t octets_send=send(clientfd,buffer,size,0);
        if(octets_send<0) return -1;
        buffer+=octets_send;
        size-=octets_send;
    }
    return 0;
}

int main(){
    int serverfd=socket(AF_INET,SOCK_STREAM,0);
    if(serverfd==-1){
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in adress;
    adress.sin_family=AF_INET;
    adress.sin_addr.s_addr=INADDR_ANY;
    adress.sin_port=htons(PORT);

    if(bind(serverfd,(struct sockaddr*)&adress,sizeof(adress))<0){
        perror("Bind server error");
        exit(EXIT_FAILURE);
    }
    if(listen(serverfd,5)<0){
        perror("Listen server error");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in client_addr;
    socklen_t addr_len=sizeof(client_addr);
    size_t size;
    int len_str_size;
    char size_str[LENGTH_MAX_NUMBER_DIGIT+1];
    printf("http://localhost:%i\n",PORT);
    while(1){
        int clientfd=accept(serverfd,(struct sockaddr*)&client_addr,&addr_len);
        if(clientfd==-1){
            perror("Accept server error");
            exit(EXIT_FAILURE);
        }
        char* content=read_file("index.html",&size);
        if(content==NULL) goto close_client;
        len_str_size=snprintf(size_str,LENGTH_MAX_NUMBER_DIGIT+1,"%lu",size);
        if(len_str_size>LENGTH_MAX_NUMBER_DIGIT){
            fprintf(stderr,"length file too large for LENGTH_MAX_NUMBER_DIGIT");
            exit(EXIT_FAILURE);
        }
        if(send_data(clientfd,request_start_line,strlen(request_start_line))<0) goto free_client;
        if(send_data(clientfd,request_headers,strlen(request_headers))<0) goto free_client;
        if(send_data(clientfd,size_str,(size_t)len_str_size)<0) goto free_client;
        if(send_data(clientfd,request_marker_body,strlen(request_marker_body))<0) goto free_client;
        if(send_data(clientfd,content,size)<0) goto free_client;
        free_client:
            free(content);
        close_client:
            close(clientfd);
    }
    close(serverfd);
}