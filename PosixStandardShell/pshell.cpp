#include "pshell.hpp"

// Objective: Execute external commands
// Steps:
// 1. Loop through each directory in path_dirs.
// 2. Construct the full path of the command using dir + '/' + arguments[0]. Example: /bin/ls
// 3. Check if the executable exists using std::ifstream
// 4. Create a child process using fork() system call.
// 5. If the child process is created successfully, then execute the command using execv() system call.
// 6. If execv() fails, then print an error message using perror().
// 7. Exit the child process.
// 8. If the parent process is created successfully, then wait for the child process to finish using waitpid() system call.
// 9. If fork() fails, then print "Fork failed".
// Inputs:
// 1. const std::vector<std::string>& arguments: A vector of strings containing command and arguments. For example, {"ls", "-l", "/usr"}.
// 2. const std::vector<std::string>& path_dirs: A vector of strings containing system path directories. For example, {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin"}.
// Intermediate Output after this function:
// 1. If the command is found in any of the directories, then the command is executed.
// 2. If the command is not found in any of the directories, then "command not found" is printed.
// Returns: None
// Example: execute_external_commands({"ls"}, {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin"}) executes the ls command.
void execute_external_commands(const std::vector<std::string>& arguments, const std::vector<std::string>& path_dirs) {
    for(const std::string& dir : path_dirs) {
        std::string filepath = dir + '/' + arguments[0]; // Construct full path of the command
        if(std::ifstream(filepath).good()) { // Check if executable exists
            pid_t pid = fork(); // Create a child process
            if(pid == 0) {
                std::vector<char*> argv; // Vector to store arguments
                for(const std::string& arg : arguments) {
                    argv.push_back(const_cast<char*>(arg.c_str())); // Convert string to char* and push to argv
                }
                argv.push_back(nullptr); // Null-terminate argument list

                execv(filepath.c_str(), argv.data()); // Execute the command
                perror("execv"); // Print error message if execv() fails
                exit(1); // Exit the child process
            }
            else if(pid > 0) {
                int status;
                waitpid(pid, &status, 0); // Wait for the child process to finish
            }
            else {
                std::cout << "Fork failed\n";
            }
            return;
        }
    }
    std::cerr << arguments[0] << ": command not found\n";
}


// Objective:  Handle builtin commands
// Steps:
// 1. Check if the first argument is "exit". If true, then exit the shell.
// 2. Check if the first argument is "pwd". If true, then get the current working directory using getcwd() system call and print it.
// 3. Check if the first argument is "cd". If true, then change the directory using chdir() system call.
// 4. Check if the first argument is "echo". If true, then print the arguments.
// 5. Check if the first argument is "type". If true, then check if the command is a shell builtin or an external command.
// 6. If the command is not a shell builtin, then execute the external command using execute_external_command(arguments, path_dirs) function.
// Inputs:
// 1. const std::vector<std::string>& arguments: A vector of strings containing command and arguments.
// 2. const std::vector<std::string>& path_dirs: A vector of strings containing system path directories.
// Intermediate Output after this function:
// 1. If the command is "exit", then the shell exits.
// 2. If the command is "pwd", then the current working directory is printed.
// 3. If the command is "cd", then the directory is changed.
// 4. If the command is "echo", then the arguments are printed. 
// 5. If the command is "type", then the command is checked if it is a shell builtin or an external command.
// 6. If the command is not a shell builtin, then the external command is executed.
// Returns: None
// Example:
// 1. handle_builtin_commands({"exit"}, {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin"}) exits the shell.
// 2. handle_builtin_commands({"pwd"}, {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin"}) prints the current working directory.
// 3. handle_builtin_commands({"cd", "/usr"}, {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin"}) changes the directory to /usr.
// 4. handle_builtin_commands({"echo", "Hello", "World"}, {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin"}) prints Hello World.
// 5. handle_builtin_commands({"type", "echo"}, {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin"}) prints echo is a shell builtin.
// 6. handle_builtin_commands({"type", "ls"}, {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin"}) prints ls is /bin/ls.
// 7. handle_builtin_commands({"ls"}, {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin"}) executes the ls command.
// 8. handle_builtin_commands({"unknown"}, {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin"}) prints unknown: command not found.
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
            std::cerr << "pwd: error getting current directory\n";
        }
    }
    else if(arguments[0] == "cd") {
        if(arguments.size() < 2) {
            std::cerr << "cd: missing argument\n";
            return;
        }
        if(arguments[1] == "~") {
            const char* home = getenv("HOME");
            if(home) {
                chdir(home);
                return;
            }
            else {
                std::cerr << "cd: HOME not set\n";
                return;
            }
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
    else if(arguments[0] == "type") {
        if(arguments.size() < 2) {
            std::cerr << "type: missing argument\n";
            return;
        }
        std::string cmd = arguments[1];
        if(cmd == "echo" || cmd == "exit" || cmd == "type" || cmd == "pwd" || cmd == "cd") {
            std::cout << cmd << " is a shell builtin\n";
        }
        else {
            for(const std::string& dir : path_dirs) {
                std::string filepath = dir + '/' + cmd;
                if(std::ifstream(filepath).good()) {
                    std::cout << cmd  << " is " << filepath << '\n';
                    return;
                }
            }
            std::cerr << "type: " << cmd << " not found\n";
        }
    }
    else {
        execute_external_commands(arguments, path_dirs);
    }

}

// Objective: Split a string by a delimeter
// Steps:
// 1. Create a stringstream object from the input string. A stringstream object is used to read from a string. Example: "Hello World" -> stringstream object.
// 2. Create a vector of strings to store tokens. A token is a substring separated by a delimeter. Example: {"Hello", "World"}. Output at this step is {"Hello", "World"}.
// 3. Create a string to store the token. Example: "Hello"
// 4. Get tokens from the stream till the delimeter. Example: "Hello World" -> "Hello" -> "World"
// 5. Return the vector of strings containing tokens. Example: {"Hello", "World"}
// Inputs:
// 1. const std::string& str: Input string to split.
// 2. char delimeter: Delimeter to split the string.
// Returns: A vector of strings containing tokens.
// Example: split_string("Hello World", ' ') returns {"Hello", "World"}
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

// Objective: Get system path directories
// Steps:
// 1. getenv("PATH") gets the PATH environment variable. Example: /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
// 2. split_string(std::string(path_env), ':') splits the PATH environment variable by ':' and returns a vector of strings. Example: {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin"}
// 3. If getenv("PATH") returns nullptr, then return an empty vector. In C++, the line std::vector<std::string>() creates an empty vector of strings like {}.
// 4. The ternary operator ? : is used to return the vector of strings if getenv("PATH") is not nullptr, otherwise return an empty vector.
// Returns: A vector of strings containing system path directories.
// Example: {"/usr/local/sbin", "/usr/local/bin", "/usr/sbin", "/usr/bin", "/sbin", "/bin"}
std::vector<std::string> get_path_dirs() {
    char* path_env = getenv("PATH");
    return (path_env) ? split_string(std::string(path_env), ':') : std::vector<std::string>();
}

// Objective: Main function to start the shell
// Steps:
// 1. Get system path directories using get_path_dirs() function.
// 2. Create a string input to store user input like commands.
// 3. Run an infinite loop to get user input.
// 4. Print the shell prompt "$ ".
// 5. Get user input using std::getline(std::cin, input).
// 6. If the input is empty, then continue to the next iteration.
// 7. Split the input string by space ' ' using split_string(input, ' ') function and store the result in a vector of strings arguments.
// 8. Handle builtin commands using handle_builtin_commands(arguments, path_dirs) function.
// Returns: None

void start_shell() {
    // Get system path directories
    std::vector<std::string> path_dirs = get_path_dirs();

    std::string input; // String to store user input like commands.
    while(true) {
        std::cout << "CustomShellbyEBP$$$ ";
        std::getline(std::cin, input);

        if(input.empty()) {
            continue;
        }

        std::vector<std::string> arguments = split_string(input, ' ');
        handle_builtin_commands(arguments, path_dirs);
    }

}

// Objective: Main function to start the shell
// Steps:
// 1. Set std::unitbuf for std::cout and std::cerr. std::unitbuf is used to flush the output buffer after each insertion.
// 2. Call start_shell() function to start the shell.
// Returns: 0
int main()
{
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    start_shell();

    return 0;
}