#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <sstream>
#include <thread>
#include <fstream>
#include "server.hpp"

void handleConnection(int client_fd) {
    char buffer[1024];

      // Receive the message
      ssize_t nbytes = recv(client_fd, buffer, 1024, 0);
      if(nbytes < 0) {
        std::cerr << "Failed to recv from client\n";
        close(client_fd);
        return;
      } 
      
      std::string request(buffer, nbytes);
      std::string response = generateHttpResponse(request);

      nbytes = send(client_fd, response.c_str(), response.length(), 0);
      if(nbytes < 0) {
        std::cerr << "Failed to send to client\n";
        close(client_fd);
        return;
      }

      close(client_fd);
      std::cout << "Connection closed\n";
}

std::vector<std::string> parseRequestHeaders(const std::string& request) {
  std::stringstream ss(request);
  std::string line;
  std::vector<std::string> headers;

  while(std::getline(ss, line) && line != "\r") {
    headers.push_back(line);
  }

  return headers;
}

std::string generateHttpResponse(const std::string& request) {
  auto headers = parseRequestHeaders(request);
  if(headers.empty()) {
    return "HTTP/1.1 400 Bad Request\r\n\r\n";
  }
  std::istringstream requestLine(headers[0]);
  std::string method, resource;
  requestLine >> method >> resource;

  if(method == "GET") {
    return handleGetRequest(resource);
  }
  else if(method == "POST") {
    std::string body = request.substr(request.find("\r\n\r\n") + 4);
    return handlePostRequest(resource, body);
  }

  return "HTTP/1.1 405 Method Not Allowed\r\n\r\n";

}

std::string handleGetRequest(const std::string& resource) {
  std::string filename = (resource == "/" ? "index.html" : resource.substr(1));
  std::ifstream file(filename);

  if(!file) {
    return "HTTP/1.1 404 Not Found\r\n\r\n";
  }

  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(content.length()) + "\r\n\r\n" + content;

  return response;
}

std::string handlePostRequest(const std::string& resource, const std::string& body) {
  // For simplicity, save to a fixed filename
  std::ofstream file("uploaded_file.txt");
  if(!file) {
    return "HTTP/1.1 500 Internal Server Error\r\n\r\n";
  }

  file << body;
  return "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 2\r\n\r\nOK";
}