#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

// Utility function to handle each client connection
void handleConnection(int client_fd);

// Utility function to parse HTTP request headers
std::vector<std::string> parseRequestHeaders(const std::string& request);

// Function to Generate an HTTP response
std::string generateHttpResponse(const std::string& request);

// Utility function to handle GET requests
std::string handleGetRequest(const std::string& resource);

// Utility function to handle POST requests
std::string handlePostRequest(const std::string& resource, const std::string& body);

#endif // SERVER_HPP