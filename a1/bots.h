#ifndef BOTS_H
#define BOTS_H

#include <semaphore.h>  /* POSIX semaphore support */
#include <pthread.h>    /* POSIX threads support */

/* Number of bot threads to spawn */
#define NUM_THREADS 7
/* Number of times each thread writes its quote */
#define NUM_ITER    8

/*
 * Global semaphore used by threads to synchronize access
 * to the shared output file QUOTES.txt.
 */
extern sem_t flag;

/*
 * Per-thread data structure. Each thread receives its own
 * thread_data containing the unique thread ID.
 */
struct thread_data {
    int id;  /* Unique thread identifier (1..NUM_THREADS) */
};

/*
 * Thread entry point. Each bot thread runs this function,
 * writing to the shared file and logging to stdout.
 * @param arg Pointer to a dynamically-allocated thread_data
 * @return NULL (threads exit after completing NUM_ITER iterations)
 */
void *bot_thread(void *arg);

#endif /* BOTS_H */