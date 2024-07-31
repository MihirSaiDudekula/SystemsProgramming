#ifndef HANDLE_REQUEST_H
#define HANDLE_REQUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ParsedRequest.h"
#include "connect_remote_server.h"
#include "add_to_cache.h"

#define MAX_BYTES 4096

int handle_request(int clientSocketId, struct ParsedRequest* request, char* tempReq);

#endif // HANDLE_REQUEST_H
