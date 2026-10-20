#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

// define structs and constants for the IPv4 & IPv6 network protocols  
//  sockaddr_in  in_port_t INADDR_ANY
#include <netinet/in.h>

typedef enum{
    CREATE_SERVER_SYS_ERR=-2,
    CREATE_SERVER_IP_ERR=-1,
}create_server_result_t;

// return serverfd or error enum
create_server_result_t CreateServer(char* interface_ip,in_port_t port,int backlog);
#endif