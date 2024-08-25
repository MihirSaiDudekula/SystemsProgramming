#include "proxy_parse.h"

#include <asm-generic/socket.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <error.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_CLIENTS 10
#define MAX_BYTES 4096    // bytes allocation space - 4KB
#define MAX_ELEMENT_SIZE 10 * (1<<10)
#define MAX_SIZE 200 * (1<<20) // size of cache

typedef struct cache_element cache_element ;

// Implementing the cache element for LRU cache (time-based)
struct cache_element {
    char* data;
    int len;
    char* url;
    time_t lru_time_track;
    cache_element* next; 
    // We need a linked-list to access corresponding cache elements
};

cache_element* find(char* url);
int add_cache_element(char* data, int size, char* url);
void remove_cache_element();

int port_number = 8080;
int proxy_socketId;

// Creating new thread for each socket connection with client
// 1. Defining max-limit = 10 (array containig thread-ids)
pthread_t tid[MAX_CLIENTS];

// 2. LRU cache is a shared resource. When multiple threads access it there 
// can be a race condition. Hence, we setup a lock.
pthread_mutex_t lock;

// 3. As we have setup a limit of 10 client connections, we might run out of 
// client connections. In which case, we need to hold the further incoming 
// requests till a connection becomes free.
sem_t semaphore;

// Global HEAD for the linkedlist which contains the LRU cache
cache_element* head;
int cache_size;

int sendErrorMessage(int socket, int status_code){
    char str[1024];
    char currentTime[50];
    time_t now = time(0);

    struct tm data = *gmtime(&now);
    strftime(currentTime, sizeof(currentTime), "%a, %d %b %Y %H:%M:%S %Z", &data);

    switch (status_code) {
        case 400:
            sprintf(str,
            "HTTP/1.1 400 Bad Request\r\n\
             Content-Length: 95\r\n\
             Connection: keep-alive\r\n\
             Content-Type: text/html\r\n\
             Date: %s\r\n\
             Server: \r\n\r\n\
             <html>\
                 <head>\
                     <title>400 Bad Request</title>\
                 </head>\n\
                 <body>\
                     <h1>400 Bad Request</h1>\n\
                 </body>\
             </html>", currentTime);
            printf("400 Bad Request\n");
            send(socket, str, strlen(str), 0);
            break;
        case 403:
            sprintf(str,
            "HTTP/1.1 403 Forbidden\r\n\
             Content-Length: 112\r\n\
             Connection: keep-alive\r\n\
             Content-Type: text/html\r\n\
             Date: %s\r\n\
             Server: \r\n\r\n\
             <html>\
                 <head>\
                     <title>403 Forbidden</title>\
                 </head>\n\
                 <body>\
                     <h1>403 Forbidden</h1>\n\
                 </body>\
             </html>", currentTime);
            printf("403 Forbidden\n");
            send(socket, str, strlen(str), 0);
            break;
        case 404:
            sprintf(str,
            "HTTP/1.1 404 Not Found\r\n\
             Content-Length: 91\r\n\
             Connection: keep-alive\r\n\
             Content-Type: text/html\r\n\
             Date: %s\r\n\
             Server: \r\n\r\n\
             <html>\
                 <head>\
                     <title>404 Not Found</title>\
                 </head>\n\
                 <body>\
                     <h1>404 Not Found</h1>\n\
                 </body>\
             </html>", currentTime);
            printf("404 Not Found\n");
            send(socket, str, strlen(str), 0);
            break;
        case 500:
            sprintf(str,
            "HTTP/1.1 500 Internal Server Error\r\n\
             Content-Length: 115\r\n\
             Connection: keep-alive\r\n\
             Content-Type: text/html\r\n\
             Date: %s\r\n\
             Server: \r\n\r\n\
             <html>\
                 <head>\
                     <title>500 Internal Server Error</title>\
                 </head>\n\
                 <body>\
                     <h1>500 Internal Server Error</h1>\n\
                 </body>\
             </html>", currentTime);
            printf("500 Internal Server Error\n");
            send(socket, str, strlen(str), 0);
            break;
        case 501:
            sprintf(str,
            "HTTP/1.1 501 Not Implemented\r\n\
             Content-Length: 103\r\n\
             Connection: keep-alive\r\n\
             Content-Type: text/html\r\n\
             Date: %s\r\n\
             Server: \r\n\r\n\
             <html>\
                 <head>\
                     <title>501 Not Implemented</title>\
                 </head>\n\
                 <body>\
                     <h1>501 Not Implemented</h1>\n\
                 </body>\
             </html>", currentTime);
            printf("501 Not Implemented\n");
            send(socket, str, strlen(str), 0);
            break;
        case 505: 
            sprintf(str,
            "HTTP/1.1 505 HTTP Version Not Supported\r\n\
             Content-Length: 125\r\n\
             Connection: keep-alive\r\n\
             Content-Type: text/html\r\n\
             Date: %s\r\n\
             Server: \r\n\r\n\
             <html>\
                 <head>\
                     <title>505 HTTP Version Not Supported</title>\
                 </head>\n\
                 <body>\
                     <h1>505 HTTP Version Not Supported</h1>\n\
                 </body>\
             </html>", currentTime);
            printf("505 HTTP Version Not Supported\n");
            send(socket, str, strlen(str), 0);
            break;
        default: return -1;
    }
    return 0;
}



int connectRemoteServer(char* host_addr, int port_num){
    // Creating remote server socket

    int remoteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(remoteSocket < 0){
        printf("Error in creating your socket\n");
        return -1;
    }
    struct hostent* host = gethostbyname(host_addr);
    if(host == NULL){
        fprintf(stderr, "No such host exists\n");
        return -1;
    }
    struct sockaddr_in server_addr;
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_num);

    bcopy((char *)&host->h_addr, (char *)&server_addr.sin_addr.s_addr, 
          host->h_length);
    // if(connect(remoteSocket, (struct sockaddr *)&server_addr, 
    //     (size_t)sizeof(server_addr) < 0)){
    //     fprintf(stderr, "Error in connecting\n");
    //     return -1;
    // }
    if (connect(remoteSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Error in connecting\n");
        return -1;
    }


    // If connected successfully, return the socket (which is an integer)
    return remoteSocket;
}

int handle_request(int clientSocketId, struct ParsedRequest* request, char* tempReq) {
    char* buf = (char*)malloc(MAX_BYTES);
    if (buf == NULL) {
        perror("Memory allocation failed");
        return -1;
    }

    // Create the request to the remote server
    snprintf(buf, MAX_BYTES,
             "GET %s %s\r\n", request->path, request->version);

    // Add required headers
    if (ParsedHeader_set(request, "Connection", "close") < 0) {
        printf("Set header key is not working\n");
    }
    if (ParsedHeader_get(request, "Host") == NULL) {
        if (ParsedHeader_set(request, "Host", request->host) < 0) {
            printf("Set Host header key is not working\n");
        }
    }
    if (ParsedRequest_unparse_headers(request, buf + strlen(buf), MAX_BYTES - strlen(buf)) < 0) {
        printf("Unparse failed\n");
    }

    int server_port = request->port ? atoi(request->port) : 80;
    int remoteSocketId = connectRemoteServer(request->host, server_port);
    if (remoteSocketId < 0) {
        perror("Error connecting to remote server");
        free(buf);
        return -1;
    }

    // Send the request to the remote server
    printf("Sending request: %s\n", buf);
    if (send(remoteSocketId, buf, strlen(buf), 0) < 0) {
        perror("Error sending request to remote server");
        close(remoteSocketId);
        free(buf);
        return -1;
    }

    // Buffer to receive data from the remote server
    char* temp_buffer = (char*)malloc(MAX_BYTES);
    if (temp_buffer == NULL) {
        perror("Memory allocation failed");
        close(remoteSocketId);
        free(buf);
        return -1;
    }

    int temp_buffer_size = MAX_BYTES;
    int temp_buffer_index = 0;
    int bytes_received;

    while ((bytes_received = recv(remoteSocketId, buf, MAX_BYTES - 1, 0)) > 0) {
        // Check if temp_buffer needs resizing
        if (temp_buffer_index + bytes_received > temp_buffer_size) {
            temp_buffer_size *= 2; // Double the size
            char* new_buffer = (char*)realloc(temp_buffer, temp_buffer_size);
            if (new_buffer == NULL) {
                perror("Memory reallocation failed");
                close(remoteSocketId);
                free(buf);
                free(temp_buffer);
                return -1;
            }
            temp_buffer = new_buffer;
        }

        // Copy the received data to temp_buffer
        memcpy(temp_buffer + temp_buffer_index, buf, bytes_received);
        temp_buffer_index += bytes_received;
    }

    if (bytes_received < 0) {
        perror("Error receiving data from remote server");
    }

    // Print the received response (for debugging)
    printf("Received %d bytes from remote server\n", temp_buffer_index);
    printf("Response: %.*s\n", temp_buffer_index, temp_buffer);

    // Handle cache and client response
    add_cache_element(temp_buffer, temp_buffer_index, tempReq);

    // Send the received data to the client
    if (send(clientSocketId, temp_buffer, temp_buffer_index, 0) < 0) {
        perror("Error sending data to client");
    }

    // Clean up
    close(remoteSocketId);
    free(buf);
    free(temp_buffer);

    return 0;
}



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


void *thread_fn(void *socketNew){
    // Using a semaphore. If the value has become negative then it waits, 
    // otherwise it proceeds.
    sem_wait(&semaphore);
    int p;
    sem_getvalue(&semaphore, &p);
    printf("Semaphore value is %d\n", p);

    // Making a pointer
    int *t = (int*) socketNew;
    // Getting the value out of the pointer
    int socket = *t;

    // Now that a thread + socket has been allocated to the client, he will 
    // start sending bytes. We need to receive them.
    int bytes_send_client, len;

    char *buffer = (char*)calloc(MAX_BYTES, sizeof(char));
    bzero(buffer, MAX_BYTES);
    // Params - 
    bytes_send_client = recv(socket, buffer, MAX_BYTES, 0);
    
    while(bytes_send_client > 0){
        len = strlen(buffer);
        // Any HTTP request ends with "\r\n\r\n", we need to compare substring
        // and see if we have reached that spot or not. If no then we must 
        // keep reading else break out of the loop
        if (strstr(buffer, "\r\n\r\n") == NULL){
            bytes_send_client = recv(socket, buffer + len, MAX_BYTES - len, 0);
        } else {
            break;
        }
    }

    // Dynamically allocating this because sizeof-character tends to 
    // differ from OS-to-OS so we cannot hardcode this value
    char *tempReq = (char *)malloc(strlen(buffer) + 1);
;
    for(int i = 0; i < strlen(buffer); i++){
        tempReq[i] = buffer[i];
    }
    struct cache_element* temp = find(tempReq);

    // if the element is found in LRU cache
    if(temp != NULL){
        int size = temp -> len / sizeof(char);
        int pos = 0;
        char response[MAX_BYTES];
        while(pos < size){
            bzero(response, MAX_BYTES);
            for(int i = 0; i < MAX_BYTES; i++){
                response[i] = temp -> data[i];
                pos++;
            }
            send(socket, response, MAX_BYTES, 0);
        }
        printf("Data retrived from the cache");
        printf("%s\n\n", response);
    }
    // if element is not found in LRU cache, then first check if the bytes
    // send by the client is greater than 0 or not (managing invalid requests)
    else if (bytes_send_client > 0){
        len = strlen(buffer);
        struct ParsedRequest* request = ParsedRequest_create();

        if(ParsedRequest_parse(request, buffer, len) < 0){
            printf("Parsing failed\n");
        } else {
            bzero(buffer, MAX_BYTES);

            if(!strcmp(request -> method, "GET")){
                if(request->host && 
                    request->path && 
                    checkHTTPversion(request->version) == 1){
                    
                    bytes_send_client = handle_request(socket, request, tempReq);
                    if(bytes_send_client == -1){
                        // Internal server error - due to main server
                        sendErrorMessage(socket, 500);
                    }
                } else {
                    // Internal server error - due to proxy server
                    sendErrorMessage(socket, 500);
                }
            } else {
                printf("This code does not support any method except GET\n");
            }
        }
        // Free everything
        ParsedRequest_destroy(request);
    } else if (bytes_send_client == 0){
        printf("Client is disconnected");
    }
    shutdown(socket, SHUT_RDWR);
    close(socket);
    free(buffer);

    // Release semaphore
    sem_post(&semaphore);
    sem_getvalue(&semaphore, &p);
    printf("Semaphore post value is %d\n", p);
    free(tempReq);

    return NULL;
}


int main(int argc, char* argv[]){
    int client_socketId, client_len;
    // When we open a socket, it returns a descriptor (same as opening files)
    struct sockaddr_in server_addr, client_addr;
    // Semaphore - minVal = 0, maxVal = 10
    sem_init(&semaphore, 0, MAX_CLIENTS);
    // Initializing lock with NULL
    pthread_mutex_init(&lock, NULL);

if (argc != 2) {
    printf("Usage: %s <port_number>\n", argv[0]);
    exit(EXIT_FAILURE);
}

int port_number = atoi(argv[1]);
if (port_number <= 0 || port_number > 65535) {
    printf("Invalid port number\n");
    exit(EXIT_FAILURE);
}



    printf("Starting Proxy server at port: %d\n", port_number);
    proxy_socketId = socket(AF_INET, SOCK_STREAM, 0);

    // If socket creation fails, it returns a negative value and exit
    if (proxy_socketId < 0) {
        perror("Failed to create a socket\n");
        exit(1);
    }

    int reuse = 1;
    if (setsockopt(proxy_socketId, SOL_SOCKET, SO_REUSEADDR, 
        (const char*)&reuse, sizeof(reuse)) < 0) {
        perror("setSockOpt failed\n");
    }
    bzero((char*)&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Port binding
    if (bind(proxy_socketId, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Port is not available");
        exit(1);
    }

    printf("Binding on port %d\n", port_number);
    int listen_status = listen(proxy_socketId, MAX_CLIENTS);
    if (listen_status < 0){
        perror("Error in listening\n");
        exit(1);
    }

    int i = 0;
    int Connected_socketId[MAX_CLIENTS];

    while(1){
        bzero((char *)&client_addr, sizeof(client_addr));
        client_len = sizeof(client_addr);
        client_socketId = accept(proxy_socketId, 
                (struct sockaddr *)&client_addr, 
                (socklen_t*)&client_len);

        if(client_socketId < 0){
            printf("Not able to connect");
            exit(1);
        } else {
            Connected_socketId[i] = client_socketId;
        }

        struct sockaddr_in* client_pt = (struct sockaddr_in *)&client_addr;
        // Extract the client address from whichever socket that was opened
        struct in_addr ip_addr = client_pt -> sin_addr;
        char str[INET_ADDRSTRLEN];
        // The function converts the address from network format to presentation
        // format. Returns null if system error occurs.
        inet_ntop(AF_INET, &ip_addr, str, INET_ADDRSTRLEN);
        printf("Client is connected with port number %d and ip address is %s\n",
               ntohs(client_addr.sin_port), str);

        // All connections are open and have been accepted by the client
        // Provide a socket to the thread so that other clients can come and 
        // then a new socket is created and given to them.
        pthread_create(&tid[i], NULL, thread_fn, (void *)&Connected_socketId[i]);
        i++;
    }

    // Deallocate the socket memory
    close(proxy_socketId);
    return 1;
}

cache_element *find(char* url){
    // finding element inside linked-list
    cache_element* site = NULL;
    int temp_lock_val = pthread_mutex_lock(&lock);
    printf("Remove cache Lock acquired %d\n", temp_lock_val);
    if(head != NULL){
        site = head;
        while(site != NULL){
            if(!strcmp(site->url, url)){
                printf("LRU time track before: %ld", site->lru_time_track);
                printf("\n URL found\n");
                site->lru_time_track = time(NULL);
                printf("LRU time track after %ld", site->lru_time_track);
                break;
            }
            site = site->next;
        }
    } else {
        printf("URL not found in cache");
    }
    temp_lock_val = pthread_mutex_unlock(&lock);
    printf("Lock is unlocked");
    return site;
}

void remove_cache_element(){
    // if cache is not empty, search for the node which has the least 
    // lru_time_track and delete it.

    cache_element *p;
    cache_element *q;
    cache_element *temp;

    // Removing elements from a linked list but with a mutex-lock
    int temp_lock_val = pthread_mutex_lock(&lock);
    printf("Lock is acquired\n");
    if(head != NULL){
        for(q=head, p=head, temp=head; q->next != NULL; q = q->next){
            if(((q->next)->lru_time_track) < (temp->lru_time_track)){
                temp = q->next;
                p = q;
            }
        }
        if(temp==head){
            head = head->next;
        } else {
            // Removing the least recently used
            p->next = temp->next;
        }

        cache_size = cache_size - (temp->len) - sizeof(cache_element) 
            - strlen(temp->url) - 1;

        // Deallocate things one by one
        free(temp->data);
        free(temp->url);
        free(temp);
    }

    temp_lock_val = pthread_mutex_unlock(&lock);
    printf("Remove cache lock\n");
}

int add_cache_element(char *data, int size, char* url){
    int temp_lock_val = pthread_mutex_lock(&lock);
    printf("Remove cache Lock acquired %d\n", temp_lock_val);
    int element_size = size + 1 + strlen(url) + sizeof(cache_element);
    if(element_size > MAX_ELEMENT_SIZE){
        // element is too big, do something else
        temp_lock_val = pthread_mutex_unlock(&lock);
        printf("Add cache lock is unlocked");
    } else {
        while(cache_size + element_size > MAX_SIZE){
            remove_cache_element();
        }
        cache_element* element = (cache_element*)malloc(sizeof(cache_element));
        element->data = (char*)malloc(size+1);
        strcpy(element->data, data);
        element->url = (char*)malloc(1 + (strlen(url) * sizeof(char)));
        strcpy(element->url, url);
        element->lru_time_track = time(NULL);
        element->next = head;
        element->len = size;

        cache_size += element_size;
        temp_lock_val = pthread_mutex_unlock(&lock);
        printf("Add cache lock is unlocked\n");
        return 1;
    }
    return 0;
}
