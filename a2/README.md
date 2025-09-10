README
======

Authors:
  - Victor Allen
  - Vince Gonzales

Usernames:
  - Victor Allen:   cssc1404@edoras.sdsu.edu (Use this for testing)
  - Vince Gonzales: cssc1422@edoras.sdsu.edu

Class:
  CS 480 - Summer 2025

Assignment:
  Assignment #2 – Experimental Shell (XSH)

Files Included (File Manifest):
  |- main.cpp       - implementation of the shell entry point and CLI loop
  |- xsh.cpp        - shell functionality (parsing, process management, piping)
  |- xsh.h          - shared declarations and prototypes
  |- Makefile       - builds the executable `dsh`
  |- README         - this file

Compile Instructions:
  1. Open a terminal and navigate to your assignment directory:
       cd ~/a2
  2. Run make:
       make
     This produces the executable `dsh`.

Operating Instructions:
  1. Ensure `dsh` is executable (if needed):
       chmod +x dsh
  2. Run the shell:
       ./dsh [command] [arguments]
     - Launches external programs (e.g., `ls`, `grep`).
     - Supports single pipe (`|`) between two commands.
     - Use `exit` to terminate the shell.

Design Decisions:
  - **Parser**: Token-based, splitting on whitespace and the pipe character.
  - **Process Management**: Used `fork()` and `execvp()`, with the parent waiting via `waitpid()`.
  - **Piping**: Implemented with `pipe()` and `dup2()` for standard I/O redirection.
  - **Modular Code**: Separated parsing, execution, and utility functions for clarity.

Lessons Learned:
  - Deepened understanding of Unix process primitives (`fork`, `exec`, `wait`).
  - Practical pipelining with file descriptor manipulation.
  - Importance of robust error checking and resource cleanup for custom shells.

Credit for Code:
  - Victor Allen:
    - xsh.h, Makefile, execute_single(), split(), validate_input(), print_prompt(), README.md
  - Vince Gonzales:
    - main.cpp, execute_commands(), parse_line(), handle_exit(), read_input(), README.md