#ifndef CONNECT_REMOTE_SERVER_H
#define CONNECT_REMOTE_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

int connectRemoteServer(char* host_addr, int port_num);

#endif // CONNECT_REMOTE_SERVER_H
