#pragma once
/** *************************************************************************************
 * Exercice 1 : coureurs
 * =====================
 *
 * Barrier structure:
 *  - creation and deletion
 *  - wait function
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <semaphore.h>

/* Barrier structures */
struct barrier {
    sem_t turnstile1;
    sem_t turnstile2;
    sem_t mutex;
    int threads;
    int count;
};

/**
 * Ititializes a barrier
 * 
 * @param threads: number of threads waited
 * 
 * @return the pointer of the barrier created
 */
struct barrier *init_barrier(int threads);

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