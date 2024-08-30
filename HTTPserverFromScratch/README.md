# HTTP Server in C++

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Project Structure](#project-structure)
4. [Getting Started](#getting-started)
5. [Code Walkthrough](#code-walkthrough)
   - [Main Server Loop](#main-server-loop)
   - [Handling Client Connections](#handling-client-connections)
   - [Parsing HTTP Requests](#parsing-http-requests)
   - [Generating HTTP Responses](#generating-http-responses)
   - [Handling GET Requests](#handling-get-requests)
   - [Handling POST Requests](#handling-post-requests)
6. [Examples](#examples)
   - [GET Request Example](#get-request-example)
   - [POST Request Example](#post-request-example)
7. [Compilation and Execution](#compilation-and-execution)
8. [Contributing](#contributing)
9. [License](#license)

## Introduction

This project implements a simple HTTP server in C++ from scratch. The server supports basic HTTP methods (`GET` and `POST`), handles multiple client connections concurrently, and parses HTTP request headers to generate appropriate responses.

The primary goal of this project is to provide a foundational understanding of how an HTTP server works at a low level. The code is organized into separate header (`.hpp`) and source (`.cpp`) files to promote modularity and maintainability.

## Features

- Start a TCP server and bind to a specified port.
- Handle concurrent client connections using threads.
- Parse HTTP request headers.
- Respond with HTTP status codes (200, 404, etc.).
- Serve files using `GET` requests.
- Handle file uploads using `POST` requests.

## Project Structure

The project is organized as follows:

```
/project-root
│
├── main.cpp           # Entry point of the application
├── server.hpp         # Header file with function declarations
├── server.cpp         # Implementation of server functions
└── README.md          # Project README file
```

## Getting Started

### Prerequisites

To compile and run this project, you need a C++ compiler that supports C++11 or later (e.g., `g++`) and `make`.

### Building the Project

To build the project, navigate to the project root directory and compile the code using the following command:

```sh
g++ -o server main.cpp server.cpp -pthread
```

### Running the Server

Run the compiled server executable:

```sh
./server
```

The server will start listening for incoming connections on port `4221`.

## Code Walkthrough

### Main Server Loop

The server starts by creating a TCP socket, binding it to a specified port, and then listening for incoming connections. The main server loop continuously accepts new connections and spawns a new thread to handle each client.

```cpp
int main(int argc, char **argv) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // ... (error handling and socket setup code)
    
    while (true) {
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd != -1) {
            std::thread(handleConnection, client_fd).detach();
        }
    }

    close(server_fd);
    return 0;
}
```

**Key Points:**
- `socket()` creates a new socket.
- `bind()` associates the socket with a specific port.
- `listen()` puts the server in a state where it listens for incoming connections.
- `accept()` accepts an incoming client connection.
- `std::thread` is used to handle each client connection concurrently.

### Handling Client Connections

The `handleConnection()` function is responsible for processing each client's request. It reads the request, parses it, generates a response, and sends the response back to the client.

```cpp
void handleConnection(int client_fd) {
    char buffer[1024];
    ssize_t nbytes = recv(client_fd, buffer, sizeof(buffer), 0);
    // ... (error handling code)

    std::string request(buffer, nbytes);
    std::string response = generateHttpResponse(request);

    nbytes = send(client_fd, response.c_str(), response.length(), 0);
    // ... (error handling and cleanup code)
}
```

**Key Points:**
- `recv()` receives data from the client.
- `send()` sends data back to the client.
- The connection is closed after the response is sent.

### Parsing HTTP Requests

The `parseRequestHeaders()` function parses the raw HTTP request to extract headers and the request line.

```cpp
std::vector<std::string> parseRequestHeaders(const std::string& request) {
    std::stringstream ss(request);
    std::string line;
    std::vector<std::string> headers;

    while (std::getline(ss, line) && line != "\r") {
        headers.push_back(line);
    }
    return headers;
}
```

**Key Points:**
- A `stringstream` is used to read the request line-by-line.
- The headers are stored in a vector of strings.

### Generating HTTP Responses

The `generateHttpResponse()` function generates the appropriate HTTP response based on the request method (`GET` or `POST`) and resource.

```cpp
std::string generateHttpResponse(const std::string& request) {
    auto headers = parseRequestHeaders(request);
    if (headers.empty()) {
        return "HTTP/1.1 400 Bad Request\r\n\r\n";
    }

    std::istringstream requestLine(headers[0]);
    std::string method, resource;
    requestLine >> method >> resource;

    if (method == "GET") {
        return handleGetRequest(resource);
    } else if (method == "POST") {
        std::string body = request.substr(request.find("\r\n\r\n") + 4);
        return handlePostRequest(resource, body);
    }

    return "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
}
```

**Key Points:**
- The function checks the HTTP method and calls the appropriate handler (`handleGetRequest()` or `handlePostRequest()`).
- If the request is invalid or the method is not allowed, it returns an error response.

### Handling GET Requests

The `handleGetRequest()` function handles `GET` requests and serves files from the server's directory.

```cpp
std::string handleGetRequest(const std::string& resource) {
    std::string filename = resource == "/" ? "index.html" : resource.substr(1);
    std::ifstream file(filename);

    if (!file) {
        return "HTTP/1.1 404 Not Found\r\n\r\n";
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(content.length()) + "\r\n\r\n" + content;
    return response;
}
```

**Key Points:**
- The root path (`/`) serves the `index.html` file.
- If the requested file is not found, a `404 Not Found` response is returned.
- If the file is found, its contents are read and returned with a `200 OK` status.

### Handling POST Requests

The `handlePostRequest()` function handles `POST` requests and saves the uploaded file data.

```cpp
std::string handlePostRequest(const std::string& resource, const std::string& body) {
    std::ofstream file("uploaded_file.txt");
    if (!file) {
        return "HTTP/1.1 500 Internal Server Error\r\n\r\n";
    }

    file << body;
    return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 2\r\n\r\nOK";
}
```

**Key Points:**
- The function saves the body of the `POST` request to a file named `uploaded_file.txt`.
- If the file cannot be created, a `500 Internal Server Error` response is returned.
- If the file is saved successfully, a `200 OK` response is returned.

## Examples

### GET Request Example

To test a `GET` request, you can use `curl`:

```sh
curl http://localhost:4221/
```

This will return the contents of `index.html` if it exists, or a `404 Not Found` error if it does not.

### POST Request Example

To test a `POST` request, you can use `curl`:

```sh
curl -X POST -d "This is a test upload" http://localhost:4221/upload
```

This will save the content `"This is a test upload"` to a file named `uploaded_file.txt` on the server.

## Compilation and Execution

To compile and run the server:

1. **Compile** the source code:

   ```sh
   g++ -o server main.cpp server.cpp -pthread
   ```

2. **Run** the server:

   ```sh
   ./server
   ```

3. The server will start listening on port `4221`. Use any HTTP client (e.g., `curl` or a web browser) to interact with the server.


# Socket Programming Functions in C++ for HTTP

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


