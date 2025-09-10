// xsh.cpp
#include "xsh.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <fcntl.h>
#include <vector>

// Print the shell prompt (USERNAME and space)
void print_prompt() {
    std::cout << USERNAME << " " << std::flush;
}

// Read a full line from standard input
std::string read_input() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

// Validate input line format: each pipeline segment must have 1-2 tokens, or "exit" command
bool validate_input(const std::string &line) {
    if (line == "exit") {
        // Allow exit command without further validation
        return true;
    }
    
    // Split line by '|' to check each pipeline segment
    std::istringstream pss(line);
    std::string segment;
    while (std::getline(pss, segment, '|')) {
        std::istringstream ss(segment);
        std::string arg;
        int count = 0;
        
        // Count tokens in segment
        while (ss >> arg) {
            if (++count > 2) {
                break;  // More than 2 tokens is invalid
            }
        }
        
        // If no tokens or more than 2 were found, it's invalid
        if (count == 0 || count > 2) {
            std::cerr << "Invalid command format\n";
            return false;
        }
    }
    return true;
}

// Check if input line is "exit" to signal shell termination
bool handle_exit(const std::string &line) {
    return line == "exit";
}

// Split a string by a delimiter and return non-empty tokens
static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string tok;
    while (std::getline(ss, tok, delim)) {
        if (!tok.empty()) {
            out.push_back(tok);
        }
    }
    return out;
}

// Parse the command line into a vector of commands, each a vector of arguments
std::vector<std::vector<std::string>> parse_line(const std::string &line) {
    std::vector<std::vector<std::string>> cmds;
    
    // Split the line into pipeline segments
    for (auto &seg : split(line, '|')) {
        std::stringstream ss(seg);
        std::string arg;
        std::vector<std::string> argv;
        
        // Tokenize on whitespace to get individual arguments
        while (ss >> arg) {
            argv.push_back(arg);
        }
        cmds.push_back(argv);
    }
    return cmds;
}

// Standard file descriptor constants
enum { STDIN_FD = 0, STDOUT_FD = 1 };

// Execute a single command with optional input/output redirection
static void execute_single(const std::vector<std::string> &args, int in_fd, int out_fd) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        // Redirect standard input if in_fd is not default
        if (in_fd != STDIN_FD) {
            dup2(in_fd, STDIN_FD);
            close(in_fd);
        }
        // Redirect standard output if out_fd is not default
        if (out_fd != STDOUT_FD) {
            dup2(out_fd, STDOUT_FD);
            close(out_fd);
        }
        
        // Prepare arguments for exec
        std::vector<char*> cargv;
        for (auto &a : args) {
            cargv.push_back(const_cast<char*>(a.c_str()));
        }
        cargv.push_back(nullptr);
        
        // Execute command: absolute path if starts with '/', else search in PATH
        if (!args.empty() && args[0].front() == '/') {
            execv(cargv[0], cargv.data());
        } else {
            execvp(cargv[0], cargv.data());
        }
        
        // If exec fails, print error message and terminate child
        perror("exec");
        exit(1);
    }
    // Parent process continues without waiting here
}

// Execute a sequence of piped commands
void execute_commands(const std::vector<std::vector<std::string>> &commands) {
    int in_fd = STDIN_FD;  // Input for first command
    int pipe_fd[2];        // File descriptors for pipe ends

    // Loop through each command in the pipeline
    for (size_t i = 0; i < commands.size(); ++i) {
        // If not the last command, create a pipe for this stage
        if (i + 1 < commands.size()) {
            pipe(pipe_fd);
        }

        // Determine output fd: either pipe write end or standard output
        int out_fd = (i + 1 == commands.size()) ? STDOUT_FD : pipe_fd[1];

        // Fork and execute this single command
        execute_single(commands[i], in_fd, out_fd);

        // Close the previous input fd in the parent
        if (in_fd != STDIN_FD) {
            close(in_fd);
        }

        // If we created a pipe, close write end and save read end for next cmd
        if (i + 1 < commands.size()) {
            close(pipe_fd[1]);
            in_fd = pipe_fd[0];
        }
    }

    // Wait for all child processes to finish
    for (size_t i = 0; i < commands.size(); ++i) {
        wait(nullptr);
    }
}




