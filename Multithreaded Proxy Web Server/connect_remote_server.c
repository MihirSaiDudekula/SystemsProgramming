#include "connect_remote_server.h"

int connectRemoteServer(char* host_addr, int port_num)
{
	// host_addr: The hostname or IP address of the remote server.
	// port_num: The port number on the remote server to which the connection will be made.

	int remoteSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (remoteSocket < 0) {
	    perror("Error in creating your socket\n");
	    return -1;
	}

	struct hostent* host = gethostbyname(host_addr);
	// gethostbyname(): This function returns a pointer to a hostent structure containing information about the host.converts address to name
	if (host == NULL) {
	    fprintf(stderr, "No such host exists\n");
	    close(remoteSocket);
	    return -1;
	}

	struct sockaddr_in server_addr;
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_num);
	bcopy((char *)&host->h_addr, (char *)&server_addr.sin_addr.s_addr, host->h_length);
	// Set to the resolved IP address from host->h_addr, using bcopy() to copy the IP address into the structure.

	if (connect(remoteSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
	    fprintf(stderr, "Error in connecting\n");
	    close(remoteSocket);
	    return -1;
	}
	// if connection was successful
	return remoteSocket;
}