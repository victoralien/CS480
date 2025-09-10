README
======

Authors:
  - Victor Allen, Vince Gonzales
  
Usernames:
  - Victor Allen: cssc1404@edoras.sdsu.edu (Use this for testing)
  - Vince Gonzales: cssc1422@edoras.sdsu.edu

Class:
  CS 480 – Summer 2025

Assignment:
  Assignment #1 – Asynchronous Threads (“bots”)

Files Included (File Manifest):
  ├── bots.c         – implementation of POSIX threads + semaphore
  ├── bots.h         – shared declarations and prototypes
  ├── Makefile       – builds the executable “bots”
  └── README         – this file

Compile Instructions:
  1. Open a terminal and navigate to your assignment directory:
       cd ~/a1
  2. Run make:
       make
     This produces the executable `bots`.

Operating Instructions:
  1. Ensure `bots` is executable:
       chmod +x bots
  2. Run the program:
       ./bots
     - Creates or truncates `QUOTE.txt`, writes the process ID.
     - Spawns 7 threads (IDs 1–7).
     - Even‐ID threads sleep 2 s; odd‐ID threads sleep 3 s.
     - Each thread writes its ID + quote to `QUOTE.txt` eight times.
     - Threads log “Thread <n> is running” to stdout.
     - After all threads finish, the semaphore is destroyed and the program prints a goodbye message.

Design Decisions:
  - **Modular main**: Factored initialization, thread creation, join, and cleanup into separate functions for clarity.
  - **Unnamed POSIX semaphore** (`sem_t`) to guard file access, preventing race conditions.
  - **Dynamic thread data**: Each thread allocates a small struct carrying its ID.
  - **Distinct quotes** for even/odd threads to visually verify interleaving in the output file.
  - **Sleep intervals** differ by parity to stagger writes.

Lessons Learned:
  - Hands‐on experience with POSIX threads and semaphores for inter‐thread synchronization.
  - Importance of modular code structure for readability and maintenance.-
  - File I/O coordination and error handling in a concurrent environment.
