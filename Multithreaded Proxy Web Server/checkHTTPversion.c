#include "checkHTTPversion.h"

int checkHTTPversion(char* msg) {
    if (strncmp(msg, "HTTP/1.1", 8) == 0 || strncmp(msg, "HTTP/1.0", 8) == 0) {
        return 1;
    }
    return -1;
}
