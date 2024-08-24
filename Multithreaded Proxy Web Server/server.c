// a custom header file we use during cache implementation
#include "proxy_parse.h"

// include all necessary header files
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

// we define a constant, max no of clients that can request the server at once
#define MAX_CLIENTS 10
#define MAX_BYTES 4096
#define MAX_SIZE 200*(1<<20)     // size of the cache, 1<<20 is equivalent to 2^20 = 1MB
#define MAX_ELEMENT_SIZE 10*(1<<20)     // and hence max size of an element in cache is 10MB

// structure for cache
struct cache {
    char* data;        // data that was cached
    int len;           // length of this data
    char* url;         // URL from where we GET request
    time_t lru_time;  // time since the data element entered to compute least recent usage
    struct cache* next; // linked-list implementation, so a next pointer
};

// cache linked list methods (defined later)
struct cache* find(char* url);
int add_to_cache(char* data, int size, char* url);
void remove_from_cache();

// server variables
int port_no = 8080;
int socket_id;
pthread_t tid[MAX_CLIENTS]; // one thread per client
sem_t semaphore;            // counting semaphore to track number of clients
pthread_mutex_t lock;       // mutex for mutual exclusion
struct cache* head;         // cache linked list head
int cache_size;             // size of the cache

// function prototypes
void* handle_client(void* client_socket);
void sendErrorMessage(int client_socket, int error_code);
void initialize_server(int* server_socket, struct sockaddr_in* server_addr, int port);
void setup_signal_handlers(void);

// initialize the server
void initialize_server(int* server_socket, struct sockaddr_in* server_addr, int port) {
    *server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (*server_socket < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    int reuse = 1;
    if (setsockopt(*server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt failed");
        close(*server_socket);
        exit(EXIT_FAILURE);
    }

    memset(server_addr, 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    server_addr->sin_addr.s_addr = INADDR_ANY;

    if (bind(*server_socket, (struct sockaddr*)server_addr, sizeof(*server_addr)) < 0) {
        perror("bind failed");
        close(*server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(*server_socket, MAX_CLIENTS) < 0) {
        perror("listen failed");
        close(*server_socket);
        exit(EXIT_FAILURE);
    }
}

// main function
int main(int argc, char *argv[]) {
    int client_socket_id;
    int client_len;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    sem_init(&semaphore, 0, MAX_CLIENTS);
    pthread_mutex_init(&lock, NULL);

    if (argc == 2) {
        port_no = atoi(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Starting Proxy server at port: %d\n", port_no);

    initialize_server(&socket_id, &server_addr, port_no);

    int i = 0;
    while (1) {
        memset(&client_addr, 0, sizeof(client_addr));
        client_len = sizeof(client_addr);

        client_socket_id = accept(socket_id, (struct sockaddr*)&client_addr, (socklen_t*)&client_len);
        if (client_socket_id < 0) {
            perror("accept failed");
            continue;
        }

        struct in_addr ip_addr = client_addr.sin_addr;
        char str[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &ip_addr, str, INET_ADDRSTRLEN) != NULL) {
            printf("Client connected: %s\n", str);
        } else {
            perror("inet_ntop failed");
        }

        sem_wait(&semaphore);

        if (pthread_create(&tid[i], NULL, handle_client, (void*)&client_socket_id) != 0) {
            perror("pthread_create failed");
        }

        pthread_detach(tid[i]);
        i = (i + 1) % MAX_CLIENTS;
    }

    close(socket_id);
    sem_destroy(&semaphore);
    pthread_mutex_destroy(&lock);

    return 0;
}

// handle client requests
void* handle_client(void* socket_fd) {
    int client_socket = *(int*)socket_fd;
    struct ParsedRequest* request = (struct ParsedRequest*)malloc(sizeof(struct ParsedRequest));
    if (request == NULL) {
        perror("Memory allocation failed for ParsedRequest");
        sendErrorMessage(client_socket, 500);
        close(client_socket);
        sem_post(&semaphore);
        return NULL;
    }

    if (ParsedRequest_create(request) < 0) {
        perror("Failed to initialize ParsedRequest");
        sendErrorMessage(client_socket, 500);
        free(request);
        close(client_socket);
        sem_post(&semaphore);
        return NULL;
    }

    char* request_buffer = (char*)malloc(MAX_BYTES);
    if (request_buffer == NULL) {
        perror("Memory allocation failed for request_buffer");
        sendErrorMessage(client_socket, 500);
        ParsedRequest_destroy(request);
        free(request);
        close(client_socket);
        sem_post(&semaphore);
        return NULL;
    }
    memset(request_buffer, 0, MAX_BYTES);

    int bytes_received = recv(client_socket, request_buffer, MAX_BYTES - 1, 0);
    if (bytes_received < 0) {
        perror("Error in receiving request");
        sendErrorMessage(client_socket, 500);
    } else {
        if (ParsedRequest_parse(request, request_buffer, bytes_received) < 0) {
            perror("Error in parsing request");
            sendErrorMessage(client_socket, 400);
        } else {
            if (handle_request(client_socket, request, request_buffer) < 0) {
                sendErrorMessage(client_socket, 500);
            }
        }
    }

    free(request_buffer);
    ParsedRequest_destroy(request);
    free(request);
    close(client_socket);
    sem_post(&semaphore);
    return NULL;
}
