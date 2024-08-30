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
6. [Header Files and Network Programming](#header-files-and-network-programming)
7. [Examples](#examples)
   - [GET Request Example](#get-request-example)
   - [POST Request Example](#post-request-example)
8. [Compilation and Execution](#compilation-and-execution)

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

This section provides a detailed explanation of the code, including the key functions and syntaxes used in building the server.

### Main Server Loop

The main server loop is the heart of the application. It creates a TCP socket, binds it to a port, and listens for incoming connections.

```cpp
int main(int argc, char **argv) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }
    
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "setsockopt failed\n";
        return 1;
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(4221);
    
    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
        std::cerr << "Failed to bind to port 4221\n";
        return 1;
    }
    
    if (listen(server_fd, 5) != 0) {
        std::cerr << "listen failed\n";
        return 1;
    }
    
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

**Key Points and Syntaxes:**

- `socket()`: Creates a new socket. The parameters specify the address family (`AF_INET` for IPv4), the socket type (`SOCK_STREAM` for TCP), and the protocol (0 for IP).
  
- `setsockopt()`: Configures the socket options. In this case, `SO_REUSEPORT` allows the server to bind to a port that was recently used.
  
- `sockaddr_in`: A structure used to specify the address family, IP address, and port number. 

- `bind()`: Associates the socket with the specified IP address and port. If `bind()` fails, it usually indicates that the port is already in use or the address is incorrect.

- `listen()`: Puts the server socket in a passive listening mode where it waits for client connections. The second argument specifies the maximum length of the queue for pending connections.

- `accept()`: Blocks and waits for an incoming connection. Once a client connects, `accept()` returns a new socket descriptor specific to that connection.

- `std::thread` and `detach()`: Spawns a new thread to handle the client connection concurrently. The `detach()` function allows the thread to run independently from the main thread.

### Handling Client Connections

The `handleConnection()` function is executed in a separate thread for each client connection. It reads the client request, processes it, and sends an appropriate HTTP response.

```cpp
void handleConnection(int client_fd) {
    char buffer[1024];
    ssize_t nbytes = recv(client_fd, buffer, sizeof(buffer), 0);
    if (nbytes < 0) {
        std::cerr << "Failed to recv from client\n";
        close(client_fd);
        return;
    }

    std::string request(buffer, nbytes);
    std::string response = generateHttpResponse(request);

    nbytes = send(client_fd, response.c_str(), response.length(), 0);
    if (nbytes < 0) {
        std::cerr << "Failed to send to client\n";
        close(client_fd);
        return;
    }

    close(client_fd);
    std::cout << "Connection closed\n";
}
```

**Key Points and Syntaxes:**

- `recv()`: Receives data from a client socket. The function takes the socket descriptor, a buffer to store the received data, the buffer size, and flags (0 for default). It returns the number of bytes received or -1 on error.

- `std::string(buffer, nbytes)`: Converts the raw character buffer into a C++ `std::string` for easier manipulation.

- `send()`: Sends data to the client. Similar to `recv()`, it takes the socket descriptor, a buffer containing the data, the buffer length, and flags.

- `close()`: Closes the socket connection. This is necessary to free up resources and allow new connections.

### Parsing HTTP Requests

Parsing HTTP requests involves breaking down the raw request string into meaningful components (request line, headers, body).

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

**Key Points and Syntaxes:**

- `std::stringstream`: A stream class to operate on strings. It allows us to read from a string as if it were an input stream.

- `std::getline()`: Reads data from the input stream (`ss` in this case) into the provided string (`line`), stopping at newline characters. It helps to extract each line from the HTTP request.

- `std::vector`: A dynamic array that can resize itself automatically. It is used to store the parsed headers.

### Generating HTTP Responses

This function determines the appropriate response based on the request method and resource path.

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

**Key Points and Syntaxes:**

- `auto`: Automatically deduces the type of the variable (`headers` in this case, which is `std::vector<std::string>`).

- `std::istringstream`: Similar to `std::stringstream`, but specifically for input string streams. It is used here to parse the request line.

- `request.substr()`: Returns a substring starting from the

 specified index. It helps extract the body content from the request.

### Handling GET Requests

The `handleGetRequest()` function serves static files from the server's directory.

```cpp
std::string handleGetRequest(const std::string& resource) {
    std::ifstream file(resource.substr(1)); // Remove leading '/'
    if (!file.is_open()) {
        return "HTTP/1.1 404 Not Found\r\n\r\n";
    }

    std::stringstream ss;
    ss << file.rdbuf(); // Read file into a string stream
    std::string content = ss.str();

    return "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(content.length()) + "\r\n\r\n" + content;
}
```

**Key Points and Syntaxes:**

- `std::ifstream`: An input file stream class used to read from files.

- `file.is_open()`: Checks if the file stream is successfully opened.

- `std::stringstream::rdbuf()`: Reads the entire file buffer into the string stream, which is then converted into a `std::string`.

### Handling POST Requests

The `handlePostRequest()` function saves data sent by clients.

```cpp
std::string handlePostRequest(const std::string& resource, const std::string& body) {
    std::ofstream file(resource.substr(1)); // Remove leading '/'
    if (!file.is_open()) {
        return "HTTP/1.1 500 Internal Server Error\r\n\r\n";
    }

    file << body;
    file.close();
    return "HTTP/1.1 201 Created\r\n\r\n";
}
```

**Key Points and Syntaxes:**

- `std::ofstream`: An output file stream class used to write to files.

- `file << body`: Writes the body content to the file. The `<<` operator appends data to the stream.

- `file.close()`: Closes the file stream. It's good practice to close files explicitly to free resources.

## Header Files and Network Programming

### Header Files Used

1. **`<iostream>`**: Provides input/output stream functionality (`std::cout`, `std::cerr`, etc.).

2. **`<cstdlib>`**: Defines several general-purpose functions, including dynamic memory management, random number generation, communication with the environment, and others.

3. **`<string>`**: Contains the C++ Standard Library string class, which provides a way to handle strings as objects with methods for various string operations.

4. **`<cstring>`**: Provides functions to manipulate C-style strings and arrays, such as `strlen()`, `strcpy()`, `strcmp()`, etc.

5. **`<unistd.h>`**: Provides access to the POSIX operating system API. It includes definitions for system calls like `read()`, `write()`, `close()`, and `sleep()`.

6. **`<sys/types.h>`**: Defines data types used in system calls. This header is necessary for using types like `size_t` and `ssize_t`.

7. **`<sys/socket.h>`**: Contains definitions of the socket API for communication between computers. It provides functions like `socket()`, `bind()`, `listen()`, `accept()`, `recv()`, and `send()`.

8. **`<arpa/inet.h>`**: Provides definitions for internet operations, such as converting IP addresses between binary and text form (`inet_ntoa()`, `inet_addr()`, etc.).

9. **`<netdb.h>`**: Contains definitions for network database operations. It provides facilities for hostname to IP resolution.

10. **`<vector>`**: Part of the C++ Standard Library, it provides the vector container, a dynamic array that can grow or shrink in size.

11. **`<sstream>`**: Provides string stream classes like `std::stringstream`, `std::istringstream`, and `std::ostringstream` that facilitate string manipulation and parsing.

12. **`<thread>`**: Provides the standard C++ thread library, allowing the creation and management of threads.

13. **`<pthread.h>`**: Provides the POSIX thread (pthreads) library for multi-threading support. In this code, `<thread>` is preferred for portability and ease of use in C++.

### Network Programming Concepts

- **Sockets**: A socket is an endpoint for communication. The `socket()` function is used to create a socket. The server and client use sockets to establish a connection.

- **Binding**: The `bind()` function assigns a local address to the socket. It is necessary to bind the server to a specific IP address and port to listen for incoming connections.

- **Listening and Accepting**: The `listen()` function puts the server in a passive mode where it waits for client requests. The `accept()` function extracts the first connection request on the queue of pending connections and creates a new socket for that connection.

- **Concurrent Connections**: The server handles multiple client connections using threads. Each client connection is handled in a separate thread, allowing multiple clients to connect and interact with the server simultaneously.

## Examples

### GET Request Example

To request a file named `index.html` from the server, you can use a tool like `curl`:

```sh
curl http://localhost:4221/index.html
```

### POST Request Example

To upload a file using a `POST` request, you can use `curl`:

```sh
curl -X POST -d @file.txt http://localhost:4221/uploaded.txt
```

## Compilation and Execution

To compile and run the server:

```sh
g++ -o server main.cpp server.cpp -pthread
./server
```
