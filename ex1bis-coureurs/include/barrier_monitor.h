#pragma once
/** *************************************************************************************
 * Exercice 1 : coureurs
 * =====================
 *
 * Barrier structure (monitor):
 *  - creation and deletion
 *  - wait function
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <pthread.h>
#include <semaphore.h>

/* Barrier structures */
struct barrier {
    unsigned threads;               // count of threads waited
    unsigned count_turnstile1;      // count of threads waiting at the first turnstile
    unsigned count_turnstile2;      // count of threads waiting at the second turnstile
    pthread_cond_t turnstile1;      // first turnstile
    pthread_cond_t turnstile2;      // second turnstile
    pthread_mutex_t barrier_lock;   // locks the access to the barrier
};

/**
 * Ititializes a barrier
 * 
 * @param threads: number of threads waited
 * 
 * @return the pointer of the barrier created
 */
struct barrier *init_barrier(unsigned threads);

/**
 * Waits the arrival of all the threads before releasing them
 * 
 * Implementation of two turnstiles to prevent threads
 * from passing several times and blocking other threads
 * 
 * @param barrier description
 */
void barrier_wait(struct barrier *barrier);

/**
 * Frees the barrier stucture
 */
void free_barrier(struct barrier *barrier);
