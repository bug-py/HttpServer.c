#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include "http_protocol/request.h"
#include "http_protocol/response.h"
// define structs and constants for the IPv4 & IPv6 network protocols  
//  sockaddr_in  in_port_t INADDR_ANY
#include <netinet/in.h>
typedef enum{
    SERVER_IP_ERR=-2,
    SERVER_SYS_ERR=-1
} http_server_state_t;
typedef void(*handle_http_t)(char* ip,in_port_t port,http_request_t* request,http_response_t* response);
http_server_state_t HTTP_SERVER_loop(char* interface_ip,in_port_t port,int backlog,handle_http_t callback);
#endif