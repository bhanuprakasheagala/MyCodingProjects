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
#include <pthread.h>

void handleConnection(int client_fd) {
    char buffer[1024];

      // Receive the message
      ssize_t nbytes = recv(client_fd, buffer, 1024, 0);
      if(nbytes < 0) {
        std::cerr << "Failed to recv from client\n";
        close(client_fd);
        return;
      } 
      
      // Parse the message
      std::string message(buffer);
      std::stringstream read(buffer);
      std::string line;
      std::vector<std::string> lines;

      while(std::getline(read, line)) {
        lines.push_back(line);
      }

      std::stringstream readLine(lines[0]);
      std::string str;
      std::vector<std::string> inputStrings;

      while(std::getline(read, str, ' ')) {
        inputStrings.push_back(str);
      }

      std::string response;
      if(inputStrings[1].rfind("/echo/", 0) == 0) {
        // Get the part of the response that needs to be echoed
        std::string echo = inputStrings[1];
        echo.replace(0, 6, "");

        // Create the response message
        response = "HTTP/1.1 200 OK \r\n";
        response.append("Content-Type: text/plain\r\n");
        response.append("Content-Length: " + std::to_string(echo.length()) + "\r\n\r\n");
        response.append(echo + "\r\n");

      }
      else if(inputStrings[1].rfind("/user-agent", 0) == 0) {
          std::string ln;
          for(int i=1; i<lines.size(); ++i) {
            ln = lines[i];
            if(ln.rfind("User-Agent: ", 0) == 0) {
              ln.replace(0, 12, "");
              ln.pop_back();
              break;
            }
          }
          // Create the response message
          response = "HTTP/1.1 200 0K\r\n";
          response.append("Content-Type: text/plain\r\n");
          response.append("Content-Length: " + std::to_string(ln.length()) + "\r\n\r\n");
          response.append(ln + "\r\n");
       }
      else if(inputStrings[1] != "/") {
        response = "HTTP/1.1 404 Not Found \r\n\r\n";
      }
      else {
        response = "HTTP/1.1 200 OK \r\n\r\n";
      }
      nbytes = send(client_fd, response.c_str(), response.length(), 0);
      if(nbytes < 0) {
        std::cerr << "Failed to connect to client\n";
        close(client_fd);
        return;
      }

      close(client_fd);
      std::cout << "Connection closed\n";
}

int main(int argc, char **argv) {
  // Create a socket for the server to listen for incoming connections
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }
  
  // Enable reuse of the port to avoid "Address already in use" errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }
  
  // Define the server address structure
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);
  
  // Bind the server socket to the specified address and port
  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }
  
  // Listen for incoming connections on the server socket
  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }
  
  // Accept incoming connection from a client
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  std::cout << "Waiting for a client to connect...\n";
  while(true) {
    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
    if(client_fd != -1) {
      std::cout << "Client connected\n";
      std::thread(handleConnection, client_fd).detach();
      //std::thread thread_obj(handleConnection, client_fd);
      //thread_obj.detach();
    }
  }
  // Close server socket
  close(server_fd);

  return 0;
}