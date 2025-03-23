#ifndef PSHELL_HPP
#define PSHELL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>

void start_shell();
std::vector<std::string> split_string(const std::string& str, char delimeter);
std::vector<std::string> get_path_dirs();
void handle_builtin_commands(const std::vector<std::string>& arguments, const std::vector<std::string>& path_dirs);
void execute_external_command(const std::vector<std::string>& arguments, const std::vector<std::string>& path_dirs);

#endif // PSHELL_HPP