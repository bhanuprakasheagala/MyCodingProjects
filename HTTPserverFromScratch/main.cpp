#include "server.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

int main(int argc, char** argv) {

    // Create a Socket for the server to listen for incoming connections
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0) {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    // Enable reuse of the port to avoid "Address already in use" errors
    int reuse = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "setsockopt failed\n";
        return 1;
    }

    // Define the server address structure
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(4221);

    // Bind the server socket to the specified address and port
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        std::cerr << "Failed to bind to port 4221\n";
        return 1;
    }

    // Listen for incoming connections on the server socket
    int connection_backlog = 5;
    if(listen(server_fd, connection_backlog) != 0) {
        std::cerr << "Listening failed\n";
        return 1;
    }

    // Accept incoming connections from clients
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    std::cout << "Waiting for clients to connect...\n";

    while(true) {
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if(client_fd != -1) {
            std::cout << "Client connected\n";
            std::thread(handleConnection, client_fd).detach();
        }
        else {
            std::cerr << "Failed to accept client connection\n";
        }
    }

    // Close server socket
    close(server_fd);

    return 0;
}