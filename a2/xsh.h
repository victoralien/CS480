// xsh.h
#ifndef XSH_H
#define XSH_H

#include <string>
#include <vector>

// Static username shown in prompt
static const std::string USERNAME = "[cssc1404@assignment02]$";

// Prints the shell prompt (USERNAME followed by a space)
void print_prompt();

// Reads an entire line from standard input
std::string read_input();

// Validates command syntax: each segment (around pipes) must have 1–2 tokens
bool validate_input(const std::string &line);

// Checks if the command is "exit" to terminate the shell
bool handle_exit(const std::string &line);

// Splits the input line into commands by '|' and tokens by whitespace
std::vector<std::vector<std::string>> parse_line(const std::string &line);

// Executes one or more commands, setting up pipes as needed
void execute_commands(const std::vector<std::vector<std::string>> &commands);

#endif