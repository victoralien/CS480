#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include "bots.h"

/* Global semaphore used to synchronize access to the shared file */
sem_t flag;

/* Two sample quotes: one for even threads, one for odd threads */
static const char *quote_even =
    "\"Controlling complexity is the essence of computer programming.\" --Brian Kernighan";

static const char *quote_odd =
    "\"Computer science is no more about computers than astronomy is about telescopes.\" --Edsger Dijkstra";

/*
 * The function executed by each bot thread.
 * It sleeps, acquires the semaphore, appends its ID and quote to the file,
 * logs to stdout, then releases the semaphore. Repeats NUM_ITER times.
 */
void *bot_thread(void *arg) {
    struct thread_data *td = arg;
    int tid = td->id;

    for (int i = 0; i < NUM_ITER; i++) {
        /* Sleep interval: even threads wait 2s, odd threads 3s */
        sleep((tid % 2 == 0) ? 2 : 3);

        /* Acquire semaphore before file access */
        sem_wait(&flag);

        /* Open file in append mode */
        FILE *f = fopen("QUOTE.txt", "a");
        if (f) {
            /* Write thread ID and appropriate quote */
            if (tid % 2 == 0) {
                fprintf(f, "Thread ID %d: %s\r\n", tid, quote_even);
            } else {
                fprintf(f, "Thread ID %d: %s\r\n", tid, quote_odd);
            }
            fclose(f);
        } else {
            perror("fopen");  /* Report file open errors */
        }

        /* Log thread activity to console */
        printf("Thread %d is running\n", tid);

        /* Release semaphore after file access */
        sem_post(&flag);
    }

    free(td);  /* Free the thread-specific data */
    return NULL;
}

/*
 * Initializes the shared output file:
 * - Creates or truncates "QUOTES.txt"
 * - Writes the current process ID
 */
void init_file() {
    FILE *f = fopen("QUOTE.txt", "w");
    if (!f) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(f, "Process ID: %d\r\n", getpid());
    fclose(f);
}

/*
 * Initializes the unnamed semaphore 'flag' to 1
 */
void init_semaphore() {
    if (sem_init(&flag, 0, 1) != 0) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
}

/*
 * Creates NUM_THREADS threads, printing a creation message for each,
 * and passing a unique ID via dynamically-allocated data.
 */
void create_threads(pthread_t threads[]) {
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Creating thread %d in main()\n", i + 1);
        struct thread_data *td = malloc(sizeof(*td));
        td->id = i + 1;
        if (pthread_create(&threads[i], NULL, bot_thread, td) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
}

/*
 * Waits (joins) for all threads to complete execution.
 */
void wait_for_threads(pthread_t threads[]) {
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

/*
 * Cleans up resources:
 * - Destroys the semaphore
 * - Prints a final goodbye message
 */
void cleanup() {
    sem_destroy(&flag);
    printf("All bots finished, Goodbye!.\n");
}

/*
 * Main entry point:
 * - Initializes file and semaphore
 * - Creates and waits for threads
 * - Performs cleanup
 */
int main(void) {
    pthread_t threads[NUM_THREADS];

    init_file();         /* Create/truncate file and write PID */
    init_semaphore();    /* Setup semaphore */
    create_threads(threads);
    wait_for_threads(threads);
    cleanup();           /* Destroy semaphore and print exit */

    return EXIT_SUCCESS;
}



