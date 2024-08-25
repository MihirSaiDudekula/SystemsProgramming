#include "connect_remote_server.h"

int connectRemoteServer(char* host_addr, int port_num)
{
    int remoteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (remoteSocket < 0) {
        perror("Error in creating socket");
        return -1;
    }

    struct hostent* host = gethostbyname(host_addr);
    if (host == NULL) {
        fprintf(stderr, "No such host exists\n");
        close(remoteSocket);
        return -1;
    }

    struct sockaddr_in server_addr;
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_num);
    memcpy((char *)&server_addr.sin_addr.s_addr, (char *)host->h_addr, host->h_length);

    if (connect(remoteSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in connecting");
        close(remoteSocket);
        return -1;
    }

    return remoteSocket;
}
