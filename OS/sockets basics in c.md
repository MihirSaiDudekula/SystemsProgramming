Socket programming in C is a way to enable communication between different processes, either on the same machine or over a network. Here are the essential components:

1. **Socket Creation**:
   - Use `socket()` to create a socket. The function takes parameters specifying the address family (e.g., `AF_INET` for IPv4), socket type (e.g., `SOCK_STREAM` for TCP), and protocol (usually 0 to use the default).

   ```c
   int sockfd = socket(AF_INET, SOCK_STREAM, 0);
   ```

2. **Binding**:
   - `bind()` associates the socket with a specific address and port. This step is crucial for servers to specify where they will listen for incoming connections.

   ```c
   struct sockaddr_in addr;
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
   addr.sin_port = htons(PORT); // Port number

   bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
   ```

3. **Listening**:
   - For servers, `listen()` puts the socket in a passive mode to wait for incoming connection requests. You specify a backlog parameter to define the maximum number of queued connections.

   ```c
   listen(sockfd, BACKLOG);
   ```

4. **Accepting Connections**:
   - Use `accept()` to accept incoming connection requests. It creates a new socket for the accepted connection and returns a new file descriptor for it.

   ```c
   int new_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
   ```

5. **Connecting**:
   - For clients, use `connect()` to establish a connection to a server socket. You need to specify the server's address and port.

   ```c
   struct sockaddr_in server_addr;
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(PORT);
   inet_pton(AF_INET, "server_ip", &server_addr.sin_addr);

   connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
   ```

6. **Sending and Receiving Data**:
   - Use `send()` and `recv()` for TCP sockets, or `sendto()` and `recvfrom()` for UDP sockets to exchange data.

   ```c
   char buffer[1024];
   recv(new_sock, buffer, sizeof(buffer), 0);
   send(new_sock, "Hello", 5, 0);
   ```

7. **Closing**:
   - After communication is complete, use `close()` to close the socket and release resources.

   ```c
   close(sockfd);
   ```

These functions form the basis of network communication in C using sockets, allowing for both client-server interactions and peer-to-peer communication.