#include "thread_fn.h"

void *thread_fn(void *socketNew)
{
	// Using a semaphore. If the value has become negative then it waits, 
    // otherwise it proceeds.
    sem_wait(&semaphore);

    //use p to store semaphore value - obtain it using sem_get value - and display it 
    int p;
    sem_getvalue(&semaphore, &p);
    printf("Semaphore value is %d\n", p);


    int *t = (int*) socketNew;
    // t , a pointer contains the address of the new socket fd
    // socket, is the value of that fd obtained by dereferencing t 
    int socket = *t;

    char *buffer = (char*)calloc(MAX_BYTES, sizeof(char));
    bzero(buffer, MAX_BYTES);

    // recv is a function that reads data from the socket. It tries to read up to MAX_BYTES bytes of data from the socket into buffer. The 0 flag means no special options are used for the read operation.

    bytes_send_client = recv(socket, buffer, MAX_BYTES, 0);

    // recv returns the number of bytes actually read from the socket, which is stored in bytes_send_client.

    while(bytes_send_client > 0) {
        len = strlen(buffer);
        // Any HTTP request ends with end-of-HTTP-request sequence denoted by this sequence of characters "\r\n\r\n", 

        // we must keep reading till we encounter this substring

        // strstr is used to look for substrings in a string
        if (strstr(buffer, "\r\n\r\n") == NULL) {
            bytes_send_client = recv(socket, buffer + len, MAX_BYTES - len, 0);
            // if the end sequence isn’t found, it continues to read from the socket. The new data is appended to the end of the current buffer content.
        } else {
            break;
        }
    }

    char *tempReq = (char *)malloc(strlen(buffer) * sizeof(char) + 1);
    // Allocates memory for tempReq, which will be used to store a copy of the data in buffer.+1 for '\0'

    // copying data from buffer to tempReq
    for(int i = 0; i < strlen(buffer); i++){
    	tempReq[i] = buffer[i];
	}

	// Checks if tempReq is present in the cache
	struct cache* temp = find(tempReq);
	// find(tempReq) is a function,    (defined later) that searches for tempReq in the cache.

	if(temp != NULL)
	{
		// Calculates the size of the cached data.
	    int size = temp->len / sizeof(char);
	    int pos = 0;
	    char response[MAX_BYTES];

	    // size: This is the total length of the data in the cache (in bytes).
	    // pos: This tracks how much data has been processed and sent so far.
	    while(pos < size){

	    	//remember, we are reusing this buffer, so we must clear after every use
	        bzero(response, MAX_BYTES);
	        for(int i = 0; i < MAX_BYTES; i++)
	        {
	            response[i] = temp->data[i];
	            // Copy data from cache to the response buffer
	            pos++;
	            // Increment the position counter
	        }
	        send(socket, response, MAX_BYTES, 0);
	        // sends the current chunk of data to the client.
	    }
	    perror("Data retrieved from the cache");
	    perror("%s\n\n", response);
	}

	// Handling Requests Not in Cache
	else if (bytes_send_client > 0){
	    len = strlen(buffer);
	    // stores the length of the string contained in the buffer.

	    struct ParsedRequest* request = ParsedRequest_create();
	    // ParsedRequest_create() allocates a new ParsedRequest object. ParsedRequest_parse() parses the HTTP request and populates the request object with parsed details.

	    if(ParsedRequest_parse(request, buffer, len) < 0)
	    	// This function parses the HTTP request contained in buffer using the length len and populates the request object with the parsed information.
	    {
	        perror("Parsing failed\n");
	    } 
	    else {
	        bzero(buffer, MAX_BYTES);

	        // strcmp gives 0 if strings are equal
	        // we are looking for get request, later we will work on post put patch delete etc
	        if(strcmp(request->method, "GET")==0){
	            if(request->host && 
	               request->path && 
	               checkHTTPversion(request->version) == 1)
	            {
	               //handle request later using a custom function handle_request
	               bytes_send_client = handle_request(socket, request, tempReq);
	               if(bytes_send_client == -1){
	                   sendErrorMessage(socket, 500);
	               }
	            } else {
	                sendErrorMessage(socket, 500);
	            }
	        } else {
	            perror("This code does not support any method except GET\n");
	        }
	    }
	    ParsedRequest_destroy(request);
	}

	else if (bytes_send_client == 0){
	    perror("Client is disconnected");
	}

	shutdown(socket, SHUT_RDWR);
	// shutdown and close the socket read-write as its use is finished
	close(socket);
	free(buffer);
	//increment semaphore allow next user
	sem_post(&semaphore);

	sem_getvalue(&semaphore, &p);
	printf("Semaphore post value is %d\n", p);
	free(tempReq);
}