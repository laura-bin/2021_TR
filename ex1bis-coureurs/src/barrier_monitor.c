/** *************************************************************************************
 * Exercice 1 : coureurs
 * =====================
 *
 * Barrier implementation (monitor):
 *  - creation and deletion
 *  - wait function
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "barrier_monitor.h"

struct barrier *init_barrier(unsigned threads) {
    // init barrier structure to 0
    struct barrier *barrier = malloc(sizeof(struct barrier));
    
    if (barrier == NULL) {
        return NULL;
    }

    memset(barrier, 0, sizeof(struct barrier));

    // except for the number of threads
    barrier->threads = threads;

    // init the conditions
    pthread_cond_init(&barrier->turnstile1, NULL);
    pthread_cond_init(&barrier->turnstile2, NULL);

    // init the mutex
    pthread_mutex_init(&barrier->barrier_lock, NULL);

    return barrier;
}

void barrier_wait(struct barrier *barrier) {
    // lock the mutex
    pthread_mutex_lock(&barrier->barrier_lock);

    // count the threads arrived at the first turnstile
    barrier->count_turnstile1++;

    // the last thread unlocks all the threads and initalize the second counter to 0
    if (barrier->count_turnstile1 == barrier->threads) {
        pthread_cond_broadcast(&barrier->turnstile1);
        barrier->count_turnstile2 = 0;
    }

    // wait for all the threads to arrive before going to turnstile 2
    while (barrier->count_turnstile1 < barrier->threads) {
        pthread_cond_wait(&barrier->turnstile1, &barrier->barrier_lock);
    }

    // repeat the same operation with the second turnstile
    barrier->count_turnstile2++;


    // the last thread unlocks all the threads and initalize the first counter to 0
    // for the next call of barrier_wait
    if (barrier->count_turnstile2 == barrier->threads) {
        pthread_cond_broadcast(&barrier->turnstile2);
        barrier->count_turnstile1 = 0;
    }
    while (barrier->count_turnstile2 < barrier->threads) {
        pthread_cond_wait(&barrier->turnstile2, &barrier->barrier_lock);
    }

    // unlock the mutex
    pthread_mutex_unlock(&barrier->barrier_lock);
}

void free_barrier(struct barrier *barrier) {
    pthread_mutex_destroy(&barrier->barrier_lock);
    pthread_cond_destroy(&barrier->turnstile1);
    pthread_cond_destroy(&barrier->turnstile2);
    free(barrier);
}
