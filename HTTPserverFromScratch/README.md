# HTTP Server from Scratch using C++

## Introduction

Welcome to another project that can take our C++ programming skills to one step better than yesterday.

Here we implement an HTTP server from scratch using C++!!

**HTTP(Hyper Text Transfer Protocol)** is the protocol that powers the web. In this project, we 'll build a HTTP server that's capable of handling simple GET/POST requests,
serving files and handling multiple concurrent connections. <br/>

Along the way, we'll learn about TCP connections, HTTP headers, HTTP verbs, handling multiple connections and more. Let's start with the basics!!

## Basics of HTTP
HTTP is a protocol for fetching resources such as HTML documents. It is the foundation of any data exchange on the Web and it is a client-server protocol, which means requests are initiated by the recipient, usually the Web browser. A complete document is reconstructed from the different sub-documents fetched, for instance, text, layout description, images, videos, scripts, and more.

![image](https://github.com/bhanuprakasheagala/MyCodingProjects/assets/31948251/86f7aad5-6089-42a7-9514-b7b66a6c7c48)

### The core concepts of HTTP include:

**Client-Server Model:** HTTP operates in a client-server architecture, where a client sends requests and a server responds with resources. <br/>
**Request Methods:** HTTP defines various request methods, such as GET, POST, PUT, DELETE, etc., each serving different purposes for data retrieval, submission, modification, or deletion. <br/>
**URLs (Uniform Resource Locators):** Resources on the web are identified by URLs. HTTP requests include URLs to specify the location of the requested resource.<br/>
**Status Codes:** HTTP responses include status codes indicating the outcome of the request. Status codes range from informational (1xx), success (2xx), redirection (3xx), client errors (4xx), to server errors (5xx).<br/>
**Headers:** HTTP requests and responses contain headers that provide additional information about the request or response, such as content type, content length, cookies, etc.<br/>

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


