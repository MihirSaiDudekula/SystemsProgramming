#include "handle_request.h"

int handle_request(int clientSocketId, struct ParsedRequest* request, char* tempReq)
{
	/*
	clientSocketId: The socket file descriptor for the client connection.
	request: A pointer to a ParsedRequest structure that contains details of the HTTP request.
	tempReq: A temporary buffer or key related to caching*/

    char* buf = (char *)malloc(sizeof(char)*MAX_BYTES);

    // creating the http request
    strcpy(buf, "GET ");
    strcat(buf, request->path);
    strcat(buf, " ");
    strcat(buf, request->version);
    strcat(buf, "\r\n");

    size_t len = strlen(buf);
    
    // Set the Connection header to "close" indicating that the server should close the connection after the response is sent
    if(ParsedHeader_set(request, "Connection", "close") < 0){
        perror("Set header key is not working");
    }


    // Ensure that the Host header is present in the request. If it is not already set, it adds the Host header with the value from request->host
    if(ParsedHeader_get(request, "Host") == NULL){
        if(ParsedHeader_set(request, "Host", request->host) < 0){
            perror("Set Host header key is not working");
        }
    }
    if(ParsedRequest_unparse_headers(request, buf + len, 
            (size_t)MAX_BYTES -  len) < 0){
        perror("Unparse failed");
    	free(buf);
    }

    // Default to port 80 (standard HTTP port). If request->port is provided, convert it to an integer and use it as the port.
    int server_port = 80;
    if(request->port != NULL){
        server_port = atoi(request->port);
    }

    // Establish a connection to the remote server (where the resource is hosted) using the connectRemoteServer function.
    int remoteSocketId = connectRemoteServer(request->host, server_port);
    if(remoteSocketId < 0)
    {
    	free(buf);
        return -1;
    }
    
    // Send the constructed HTTP request to the remote server using the send function. Clear buf after sending.
    int bytes_send = send(remoteSocketId, buf, strlen(buf), 0);
    bzero(buf, MAX_BYTES);

     // Receive the response from the remote server. Initialize temp_buffer to store the entire response data and set its initial size.
    bytes_send = recv(remoteSocketId, buf, MAX_BYTES - 1, 0);

    char* temp_buffer = (char*)malloc(sizeof(char)*MAX_BYTES);
    int temp_buffer_size = MAX_BYTES;
    int temp_buffer_index = 0;

    while(bytes_send > 0){
        bytes_send = send(clientSocketId, buf, bytes_send, 0);
        for(int i = 0; i < bytes_send/sizeof(char); i++){

            // Accept all incoming responses into temp buffer as this has to 
            // be stored in the LRU cache later on
            temp_buffer[temp_buffer_index] = buf[i];
            temp_buffer_index++;
        }
        temp_buffer_size += MAX_BYTES;
        // If more space is needed in temp_buffer, it is dynamically reallocated.
        temp_buffer = (char*)realloc(temp_buffer, temp_buffer_size);
        if(bytes_send < 0)
        {
            perror("Error in sending data to the client\n");
            break;
        }
        bzero(buf, MAX_BYTES);
        bytes_send = recv(remoteSocketId, buf, MAX_BYTES - 1, 0);
    }

    // Release the memory allocated for buf.
    free(buf);
    // Store the received data in the cache using add_cache. This likely adds the response data to a cache for future requests.
    add_cache(temp_buffer, strlen(temp_buffer), tempReq);
    free(temp_buffer);
    close(remoteSocketId);

    return 0;
    
}