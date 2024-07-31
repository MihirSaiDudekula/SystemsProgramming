// a custom header file we use during cache implementation
#include "proxy_parse.h"

//include all necessary header files
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


// we define a constant, max no of clients that can reques the server at once
#define MAX_CLIENTS 10
#define MAX_BYTES 4096
#define MAX_SIZE 200*(1<<20)     
//size of the cache , 1<<20 is equivalent to 2^20 = 1MB
// so the max cache size is 200MB
#define MAX_ELEMENT_SIZE 10*(1<<20)     
// and hence max size of an element in cache is 10MB

struct cache 
{
	//data that was cached
	char* data;

	//length of this data
	int len;

	//URL from where we GET request
	char* url;

	/*time since the data element entered to compute least recent usage*/
	time_t lru_time;

	//implementation as a linked-list, so a next pointer
	struct cache* next;
};

//cache LL methods (defined later)

//method to traverse linked list for finding a matching URL

struct cache* find(char* url);

// add to our cache a new node when cache miss
int add_to_cache(char* data,int size,char* url);

void remove_from_cache();

// we are working with sockets, so some necessary vars

int port_no = 8080;
int socket_id;

//one thread per client, so array of threads 
pthread_t tid[MAX_CLIENTS]; 

//cache is a shared resource between threads, so must allow exclusive access

//use a counting semaphore to keep track of number of clients incoming
sem_t semaphore;

// lock for mutual exclustion (mutex)
pthread_mutex_t lock;

//cache linked list head and size
struct cache* head;
int cache_size;
int bytes_send_client;
size_t len;

int main(int argc, char *argv[])
{
	int client_socket_id;
	// an integer variable used to store the file descriptor for a socket,later When you create a socket using socket(),

	int client_len;
	// used to specify the length of the sockaddr structure

	// struct sockaddr_in is a structure used in socket programming in C to define an endpoint address for network communication

	//it contains
	// Address family (e.g., AF_INET for IPv4), Port number and IP address

	// so we use 2 structs, for storing the address information of a client
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;



    // Semaphore - minVal = 0, maxVal = 10 - we declared 10 as max earlier
    sem_init(&semaphore, 0, MAX_CLIENTS);

    // Initializing lock with NULL
    pthread_mutex_init(&lock, NULL);

    // if the count of number of arguments passed is 2, only then accept it else we cannot proceed

    if(argc == 2){
    	// while executing, we need to use the following command in the terminal

        // ./sevrer 9090 

        // so, it takes two args, the program name and the port number as the argument
        
        port_no = atoi(argv[1]);
        // atoi converts the string argument

    } else {
        perror("Too few arguments");
        exit(1);
    }

    perror("Starting Proxy server at port: %d\n", port_no);

    // socket() is the function used to create a socket. The function takes parameters specifying the address family (e.g., `AF_INET` for IPv4), socket type (e.g., `SOCK_STREAM` for TCP), and protocol (usually 0 to use the default)

    // returns a file descriptor socket_id
    socket_id = socket(AF_INET, SOCK_STREAM, 0);

    // If socket creation fails, it returns a -1, so based on that, we can exit
    if (socket_id < 0) {
        perror("Failed to create a socket\n");
        exit(1);
    }

    int reuse = 1;
    // reuse toggles between 1 and 0
     // When you set reuse to 1 and pass it to setsockopt, you are telling the system to reuse local addresses, which is particularly useful in scenarios like quickly restarting a server without waiting for the operating system to release the port.

    // If you set reuse to 0, you would be disabling the SO_REUSEADDR

    if (setsockopt(
    	socket_id, 
    	SOL_SOCKET, 
    	SO_REUSEADDR, 
        (char*)&reuse, 
        sizeof(reuse)) < 0) 
    {
        perror("setSockOpt failed\n");
    }

    // setsockopt function is used to configure various options for a socket.

    // the basic prototype goes as such

    // int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

    // sockfd : This is the file descriptor for the socket you want to configure, here socket_id

    // level: specifies the protocol level at which the option is defined.SOL_SOCKET, which means that the option is a generic socket option, not specific to a particular protocol (like TCP or UDP).

    // optname:This specifies the particular option you want to set. For example, SO_REUSEADDR is an option that allows you to reuse local addresses.

    // optlen: specifies the length of the option value. It is used to tell the function how much memory is pointed to by optval. For instance, if optval points to an integer, you would pass sizeof(int) here.

    bzero((char*)&server_addr, sizeof(server_addr));
    // initializes the server_addr structure to zero to ensure no garbage values are present.

    server_addr.sin_family = AF_INET;
    // Sets the address family to IPv4.
    server_addr.sin_port = htons(port_no);
    // htons() Converts the port number to network byte order (big-endian) - converts port no to network understandable format in simple terms
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // INADDR_ANY, which means the server will accept connections on any network interface.

    // Port binding
    // bind() associates the socket with a specific address and port. This step is crucial for servers to specify where they will listen for incoming connections.

    if(bind(
    	socket_id, 
        (struct sockaddr*)&server_addr,
         sizeof(server_addr) < 0))
	    {
	        perror("Port is not available");
	        exit(1);
	    }

	// Functions like bind(), connect(), accept(), and others in the socket API expect a pointer to struct sockaddr as an argument.

	 /*wrt:
	 (struct sockaddr*)&server_addr*/

	// By typecasting &server_addr to (struct sockaddr*), you are effectively converting the specific address structure (struct sockaddr_in or similar) into the more general struct sockaddr type. This allows you to pass it to functions that operate with struct sockaddr pointers.

    perror("Binding on port %d\n", port_no);

    // when you call listen(), the socket enters a state where it is waiting for other clients to connect to it.
    int listen_status = listen(socket_id, MAX_CLIENTS);
    // the second parameter here is the Backlog Parameter: This parameter specifies how many connection requests can be waiting in a queue if the server is too busy to handle them immediately, in this case, MAX_CLIENTS

    if (listen_status < 0){
        perror("Error in listening\n");
        exit(1);
    }

    int i = 0;
    int Connected_socket_id[MAX_CLIENTS];
    // i is used to index the Connected_socket_id array, where you store the file descriptors of connected client sockets

    while(1)
    {
    	// here we accept connections from clients, hece this loop need to run forever 

    	// init to 0
        bzero((char *)&client_addr, sizeof(client_addr));

        //set client length
        client_len = sizeof(client_addr);

        // `accept()` to accept incoming connection requests. It creates a new socket for the accepted connection and returns a new file descriptor for it.
        client_socket_id = accept(socket_id, 
            (struct sockaddr *)&client_addr,
            (socklen_t *)&client_addr
        );
        if(client_socket_id < 0){
            perror("Not able to connect");
            exit(1);
        } 
        else {
        	//add the fd of the connected socket to array
            Connected_socket_id[i] = client_socket_id;
        }

    struct sockaddr_in* client_pt = (struct sockaddr_in *)&client_addr;
    // To access specific information about the client (like IP address and port), you cast client_addr to struct sockaddr_in*, which is a more specific structure that contains the client’s address information.
        // Extract the client address from whichever socket that was opened
        struct in_addr ip_addr = client_pt -> sin_addr;

        // declares a character array to hold the human-readable string representation of the IP address. INET_ADDRSTRLEN
        char str[INET_ADDRSTRLEN];


        // inet_ntop is a function that converts the IP address from network format (binary) to presentation format (string).
        // Returns null if system error occurs.
        inet_ntop(AF_INET, &ip_addr, str, INET_ADDRSTRLEN);


        perror("Client is connected with port number %d and ip address is %s\n",
               ntohs(client_addr.sin_port), str);

        // All connections are open and have been accepted by the client
        // Provide a socket to the thread so that other clients can come and 
        // then a new socket is created and given to them.
        pthread_create(
        	&tid[i], 
        	NULL, 
        	thread_fn, 
        	(void *)&Connected_socket_id[i]);

        // creates a new thread. &tid[i] is a pointer to the thread identifier. NULL specifies default thread attributes. thread_fn is the function that the new thread will execute. (void *)&Connected_socketId[i] is the argument passed to thread_fn, which is the identifier of the connected socket.

        i++; 
         // increments the index i, preparing for the next connection.
    }
    // Deallocate the socket memory
    close(proxy_socketId);
    return 1;    
}

