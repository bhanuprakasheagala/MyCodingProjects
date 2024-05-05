# HTTP Server from Scratch using C++

## Introduction

Welcome to another project that can take our C++ programming skills to one step towards mastery. This time we are doing Network programming.

HTTP Server Request Response is the basic concept of modern web that is a simple and elegant. Just browse HTTP Server, you'll hit a lots of information in web!!

You can read all the basics about HTTP Server at <a href="https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview"> HTTP Overview </a>

In this Project we are going to dive into the implementation of HTTP server. On the way, we focus on learning the following concepts of HTTP Protocol:

1. Start a TCP server on a Port (Bind to a Port)
2. Respond with 200
3. Respond with 404
4. Respond with content
5. Parse headers
6. Concurrent connections
7. GET a file
8. POST a file

We will try to cover each of the above step and implement it. At the end, we have our HTTP Server with all the basic functionalities.

Happy learning!!!

**Socket Programming Functions in C++ for HTTP:**

1. **Socket Creation (socket):**
   - **Description:** The `socket()` function is used to create a socket, which is an endpoint for communication. It takes three arguments: domain (e.g., AF_INET for IPv4), type (e.g., SOCK_STREAM for TCP), and protocol (usually 0 for default protocol).
   - **Syntax in C++:** `int socket(int domain, int type, int protocol);`
   - **Example Code Usage:**
     ```cpp
     #include <sys/socket.h>
     #include <iostream>
     
     int main() {
         int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
         if (serverSocket == -1) {
             std::cerr << "Error creating socket\n";
             return 1;
         }
         std::cout << "Socket created successfully\n";
         // Further code for binding, listening, etc.
         return 0;
     }
     ```

2. **Binding (bind):**
   - **Description:** The `bind()` function associates a socket with a specific IP address and port number on the local machine. It takes the socket file descriptor, a sockaddr structure containing the address details, and the size of the address structure.
   - **Syntax in C++:** `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`
   - **Example Code Usage:**
     ```cpp
     #include <sys/socket.h>
     #include <netinet/in.h>
     #include <iostream>
     
     int main() {
         struct sockaddr_in serverAddr;
         serverAddr.sin_family = AF_INET;
         serverAddr.sin_port = htons(8080); // Example port
         serverAddr.sin_addr.s_addr = INADDR_ANY;
     
         int bindStatus = bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
         if (bindStatus == -1) {
             std::cerr << "Error binding socket\n";
             return 1;
         }
         std::cout << "Socket bound successfully\n";
         // Further code for listening, accepting, etc.
         return 0;
     }
     ```

3. **Listening (listen):**
   - **Description:** The `listen()` function puts the server socket into a passive mode, waiting for incoming connections from clients. It takes the socket file descriptor and the maximum number of connections that can be queued for processing.
   - **Syntax in C++:** `int listen(int sockfd, int backlog);`
   - **Example Code Usage:**
     ```cpp
     #include <sys/socket.h>
     #include <iostream>
     
     int main() {
         int listenStatus = listen(serverSocket, 5); // Example backlog size
         if (listenStatus == -1) {
             std::cerr << "Error listening on socket\n";
             return 1;
         }
         std::cout << "Listening for connections\n";
         // Further code for accepting connections, etc.
         return 0;
     }
     ```

4. **Accepting Connections (accept):**
   - **Description:** The `accept()` function is called by a server to accept a connection request from a client. It creates a new socket for the connection and returns a new file descriptor representing the connected socket.
   - **Syntax in C++:** `int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);`
   - **Example Code Usage:**
     ```cpp
     #include <sys/socket.h>
     #include <iostream>
     
     int main() {
         struct sockaddr_in clientAddr;
         socklen_t clientAddrLen = sizeof(clientAddr);
         int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
         if (clientSocket == -1) {
             std::cerr << "Error accepting connection\n";
             return 1;
         }
         std::cout << "Connection accepted from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
         // Further code for sending/receiving data, etc.
         return 0;
     }
     ```

5. **Sending Data (send) and Receiving Data (recv):**
   - **Description:** The `send()` function is used to send data over a connected socket, and the `recv()` function is used to receive data from a connected socket. Both functions take the socket file descriptor, a buffer containing the data, the size of the buffer, and optional flags.
   - **Syntax in C++:** `ssize_t send(int sockfd, const void *buf, size_t len, int flags);` and `ssize_t recv(int sockfd, void *buf, size_t len, int flags);`
   - **Example Code Usage (Sending):**
     ```cpp
     #include <sys/socket.h>
     #include <iostream>
     
     int main() {
         const char *message = "Hello, client!";
         int sendStatus = send(clientSocket, message, strlen(message), 0);
         if (sendStatus == -1) {
             std::cerr << "Error sending data\n";
             return 1;
         }
         std::cout << "Data sent successfully\n";
         // Further code for receiving data, etc.
         return 0;
     }
     ```
   - **Example Code Usage (Receiving):**
     ```cpp
     #include <sys/socket.h>
     #include <iostream>
     
     int main() {
         char buffer[1024];
         int recvSize = recv(clientSocket, buffer, sizeof(buffer), 0);
         if (recvSize == -1) {
             std::cerr << "Error receiving data\n";
             return 1;
         }
         buffer[recvSize] = '\0'; // Null-terminate the received data
         std::cout << "Received data: " << buffer << std::endl;
         // Further code for processing data, etc.
         return 0;
     }
     ```

6. **Closing Sockets (close):**
   - **Description:** The `close()` function is used to close a socket when communication is complete. It takes the socket file descriptor as an argument.
   - **Syntax in C++:** `int close(int sockfd);`
   - **Example Code Usage:**
     ```cpp
     #include <unistd.h>
     
     int main() {
         close(clientSocket);
         close(serverSocket);
         return 0;
     }
     ```

These socket programming functions in C++ provide the foundation for building HTTP servers capable of handling client requests and serving resources efficiently.


