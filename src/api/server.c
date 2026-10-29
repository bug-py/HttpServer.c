#include "api/server.h"
#include "struct_data/safe_alloc.h"
#include "parser/parser.h"
#include <stdio.h>
#include <stdlib.h>

// UNIX syscall 
// close() write() read()
#include <unistd.h>

// socket syscall 
// socket() bind() setsockopt()
#include <sys/socket.h>

// conversion to internet format
// PORT => htons() ntohs() 
// IP => inet_pton() inet_ntop()  INET_ADDRSTRLEN
#include <arpa/inet.h>

// multithreading for handling multiple clients
// this is not the most efficient, but the next version will use epoll() 
#include <pthread.h>

#define LEN_BUFFER_READ 1024
#define LEN_BUFFER_SEND 1024


typedef enum{
    CREATE_SERVER_SYS_ERR=-2,
    CREATE_SERVER_IP_ERR=-1,
}create_server_result_t;

typedef struct{
    int fd;
    char ip[INET_ADDRSTRLEN];
    in_port_t port;
    handle_http_t callback;
}client_t;


// return serverfd or error enum
create_server_result_t HTTP_SERVER_Create(char* interface_ip,in_port_t port,int backlog){
    // TCP & IPV4
    int serverfd=socket(AF_INET,SOCK_STREAM,0);
    if(serverfd<0) return CREATE_SERVER_SYS_ERR;
    
    // to restart the server quickly
    int opt=1;
    if(setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))<0) goto err_sys;
    
    // define the server port and interface
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    switch(inet_pton(AF_INET,interface_ip,&addr.sin_addr)){
        case 1: break;
        case 0 : goto err_ip;
        case -1 : goto err_sys;
    }
    addr.sin_port=htons(port);
    if(bind(serverfd,(struct sockaddr*)&addr,sizeof(addr))<0) goto err_sys;

    // declares the server
    if(listen(serverfd,backlog)<0) goto err_sys;

    return serverfd;
    // handle err
    err_sys:
        close(serverfd);
        return CREATE_SERVER_SYS_ERR;
    err_ip:
        close(serverfd);
        return CREATE_SERVER_IP_ERR;
}
http_request_t* recv_request(int clientfd){
    parser_context_t ctx;
    PARSER_init(&ctx,MAX_LENGTH_REQUEST);
    char buffer[LEN_BUFFER_READ];
    for(;;){
       ssize_t bytes_read=read(clientfd,buffer,LEN_BUFFER_READ);
       if(bytes_read<=0) goto clean;

        switch(PARSER_feed(&ctx,buffer,bytes_read)){
            case PARSING_SUCCES: 
                http_request_t* request=PARSER_get_request(&ctx);
                PARSER_free(&ctx);
                return request;

            case PARSING_NEED_MORE_DATA: break;

            case PARSING_ERR:
            case PARSING_OVERFLOW:    
            default:
                goto clean;
        }
    }
    clean:
        PARSER_free(&ctx);
        return NULL;

}
void* handle_client_worker(void* arg){
    client_t* client=(client_t*)arg;
    printf("%s:%i\n",client->ip,client->port);
    goto clean;
    clean:
        close(client->fd);
        free(client);
        return NULL;
}
http_server_state_t HTTP_SERVER_loop(char* interface_ip,in_port_t port,int backlog,handle_http_t callback){
    // get serverfd handle err
    int serverfd=HTTP_SERVER_Create(interface_ip,port,backlog);
    switch(serverfd){
        case CREATE_SERVER_SYS_ERR: return SERVER_SYS_ERR;
        case CREATE_SERVER_IP_ERR: return  SERVER_IP_ERR;
        default : break;
    }
    for(;;){
         // accept client
        struct sockaddr_in addr_client;
        socklen_t len_addr=sizeof(addr_client);
        int clientfd=accept(serverfd,(struct sockaddr*)&addr_client,&len_addr);
        if(clientfd<0) goto err_sys;

        // create client struct
        client_t* client=xalloc(sizeof(client_t),1,NULL);
        client->fd=clientfd;
        inet_ntop(AF_INET,&addr_client.sin_addr,client->ip,INET_ADDRSTRLEN);
        client->port=ntohs(addr_client.sin_port);
        client->callback=callback;

        // create thread and submit client
        pthread_t thread;
        if(pthread_create(&thread,NULL,&handle_client_worker,client)<0) goto err_sys;
        pthread_detach(thread);
    }
    // never reach 
    return 0;
    // handle err
    err_sys:
        close(serverfd);
        return SERVER_SYS_ERR;
}