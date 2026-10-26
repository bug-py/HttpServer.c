#include "api/server.h"
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

typedef enum{
    CREATE_SERVER_SYS_ERR=-2,
    CREATE_SERVER_IP_ERR=-1,
}create_server_result_t;

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
http_server_state_t HTTP_SERVER_loop(char* interface_ip,in_port_t port,int backlog,handle_http_t callback){
    // get serverfd handle err
    callback=callback; // for -Werror flag
    int serverfd=HTTP_SERVER_Create(interface_ip,port,backlog);
    switch(serverfd){
        case CREATE_SERVER_SYS_ERR: return SERVER_SYS_ERR;
        case CREATE_SERVER_IP_ERR: return  SERVER_IP_ERR;
        default : break;
    }
    // accept client
    struct sockaddr_in addr_client;
    socklen_t len_addr=sizeof(addr_client);
    int clientfd=accept(serverfd,(struct sockaddr*)&addr_client,&len_addr);
    if(clientfd<0) goto err_sys;


    // handle err
    err_sys:
        close(serverfd);
        return SERVER_SYS_ERR;
    return 0;
}