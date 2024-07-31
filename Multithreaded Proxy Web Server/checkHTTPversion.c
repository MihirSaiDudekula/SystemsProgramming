#include "checkHTTPversion.h"

int checkHTTPversion(char* msg){
    int version = -1;

    if(strncmp(msg, "HTTP/1.1", 8) == 0){
        version = 1;
    } else if(strncmp(msg, "HTTP/1.0", 8) == 0){
        version = 1;
    } else {
        version = -1;
    }
    return version;
}