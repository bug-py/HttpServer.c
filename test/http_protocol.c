#include <stdio.h>
#include "http_protocol/request.h"
#include "http_protocol/response.h"
int main(){
    printf("404 reason '%s'\n",status_code_to_str(404));
    printf("Method : %s\n",method_to_str(GET));
    return 0;
}