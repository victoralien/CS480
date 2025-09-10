// main.cpp
#include "xsh.h"

int main() {
    // Display the shell prompt
    print_prompt();
    while (true) {
        // Read a full line of user input
        std::string line = read_input();

        // Validate the input (checks piping and argument count)
        if (!validate_input(line)) {
            print_prompt();        // Invalid format: re-prompt and continue
            continue;
        }

        // Handle "exit" command: break out of the loop if requested
        if (handle_exit(line)) break;

        // Parse the line into a sequence of commands (with their args)
        auto commands = parse_line(line);

        // Execute the parsed commands (handles piping)
        execute_commands(commands);

        // After execution, display prompt again
        print_prompt();
    }
    return 0;            // Exit with status 0
}