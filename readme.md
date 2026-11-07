#  BASIC HTTP SERVER v0.9
#### The goal of this project is to create an HTTP/1.1 server in C. 

## FEATURES 
#### The server supports the GET method.
#### The server can parse and retrieve the URL, but not the headers.
#### The server can construct and send a response containing a status code, a reason phrase, and 3 headers:
#### - Content-Length
#### - Content-Type
#### - Server

## COMPILATION
#### This code works on gcc 13.3.0 with linux.
#### You can tape this command with make for compile lib :
```bash
make all
```
#### The lib is located in this [folder](/build/lib/).
#### You can tape this command for compile all tests :
```bash
make test
```
#### The tests are in this [folder](/build/bin/).

#### You can tape this command to compile and launch a web server that serves this [static folder](/static/) :
```bash
make http_server
```
## DOCUMENTATION
#### *the function is present in [server.h](/include/api/server.h)*
### HTTP_SERVER_LOOP()
```c
http_server_state_t HTTP_SERVER_loop(char* interface_ip,in_port_t port,int backlog,handle_http_t callback);
```
#### For this version, this is the only function exposed by the library.
#### It allows you to launch an HTTP server that runs indefinitely. 
#### It only returns a value in the event of an error.

#### *ARGUMENTS*
- #### *interface_ip*
#### This is the IPv4 address of the interface on which the server is listening.
- #### *port*
#### This is the server's listening port.
- #### *backlog*
#### This is the number of requests that can be placed in a queue before the server accepts them. 
#### It is passed as an argument to the *listen* system call.
- #### *callback*
#### This is the processing function that the server will call for each request in order to obtain a response.
#### the function is defined by :
```c
http_response_t* handle(char* ip,in_port_t port,http_request_t* request);
```
#### ip is the IPv4 address of the client computer, and port is its port.

#### It is not the function's responsibility to deallocate the request and its content.
#### However, it must allocate the response on the heap and return a valid pointer or NULL (in which case the connection will be closed).
#### Each attribute of the response must be allocated on the heap or marked as null. 
#### You can use [RESPONSE_create()](/include/http_protocol/response.h).

#### This server uses threads for each client
#### be careful with the race conditions! 

#### *RETURN VALUE*
#### This function returns a value from the [http_server_state_t enum](/include/api/server.h) only in the event of an error.
- ####  *SERVER_SYS_ERR*
#### A system error has occurred
#### The error is stored in errno.
- ####  *SERVER_IP_ERR*
#### The IP address provided for the interface is invalid.

## v0.9 
- [x] Project directory structure 
- [x] Parser that supports the GET method and the URLs
- [x] HTTP response generator that supports content-length and content-type
- [x] Blocking socket with multithreading
- [x] HTTP service that exposes a directory

## FOR NEWS VERSIONS 
- Handle POST requests and headers
- Implement Parsing Zero-copy 
- Set up a multiplexing service with epoll
