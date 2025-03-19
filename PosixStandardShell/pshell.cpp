#include "pshell.hpp"

// Handle builtin commands
void handle_builtin_commands(const std::vector<std::string>& arguments, const std::vector<std::string>& path_dirs) {
    if(arguments[0] == "exit") {
        exit(0); // Exit the shell. This exit is system call exit().
    }
    else if(arguments[0] == "pwd") {
        char cwd[1024]; // 1024 is the maximum length of a path in Linux
        if(getcwd(cwd, sizeof(cwd)) != NULL) {  // getcwd() is a system call to get the current working directory
            std::cout << cwd << std::endl;
        }
        else {
            perror("pwd"); // perror() is a system call to print the error message.
        }
    }
    else if(arguments[0] == "cd") {
        if(arguments.size() < 2) {
            std::cerr << "cd: missing argument\n";
            return;
        }
        if(chdir(arguments[1].c_str()) != 0) { // Attempt to change the directory
            perror("cd"); // Print error message if chdir() fails
        }
    }
    else if(arguments[0] == "echo") {
        for(size_t i = 1; i < arguments.size(); i++) {
            std::cout << arguments[i] << (i == arguments.size() - 1 ? "\n" : " ");
        }
    }

}

std::vector<std::string> split_string(const std::string& str, char delimeter) {
    std::stringstream ss(str); // Turn string into a stream object because getline() works with streams
    std::vector<std::string> tokens; // Vector to store tokens
    std::string token; // String to store token

    // Get tokens from the stream till the delimeter
    while(getline(ss, token, delimeter)) {
        tokens.push_back(token);
    }

    return tokens;
}

// Get system path directories
std::vector<std::string> get_path_dirs() {
    char* path_env = getenv("PATH");
    return (path_env) ? split_string(std::string(path_env), ':') : std::vector<std::string>();
}
void start_shell() {
    // Get system path directories
    std::vector<std::string> path_dirs = get_path_dirs();

    std::string input; // String to store user input like commands.
    while(true) {
        std::cout << "$ ";
        std::getline(std::cin, input);

        if(input.empty()) {
            continue;
        }

        std::vector<std::string> arguments = split_string(input, ' ');
        handle_builtin_commands(arguments, path_dirs);
    }

}

int main()
{
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    start_shell();

    return 0;
}