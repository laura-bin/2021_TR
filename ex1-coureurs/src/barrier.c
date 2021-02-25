#include <stdlib.h>
#include <stdio.h>

#include "barrier.h"

struct barrier *init_barrier(int threads){
    struct barrier *barrier = malloc(sizeof(struct barrier));
    barrier->count = 0;
    barrier->threads = threads;

    // the semaphores are not shared with other processes
    sem_init(&barrier->mutex, 0, 1);
    sem_init(&barrier->turnstile1, 0, 0);
    sem_init(&barrier->turnstile2, 0, 0);

    return barrier;
}

void barrier_wait(struct barrier *barrier) {
    int i;

    // lock the mutex
    sem_wait(&barrier->mutex);

    // the last thread arriving loads the turnstile
    // with the number of threads to let them continue
    barrier->count += 1;
    if (barrier->count == barrier->threads) {
        for (i = 0; i < barrier->threads; i++) {
            sem_post(&barrier->turnstile1);
        }
    }

    // unlock the mutex
    sem_post(&barrier->mutex);

    // each thread decrements the first turnstile
    sem_wait(&barrier->turnstile1);

    // repeat the same operation with the second turnstile
    sem_wait(&barrier->mutex);
    barrier->count -= 1;
    if (barrier->count == 0) {
        for (i = 0; i < barrier->threads; i++) {
            sem_post(&barrier->turnstile2);
        }
    }
    sem_post(&barrier->mutex);
    sem_wait(&barrier->turnstile2);
}

void free_barrier(struct barrier *barrier) {
    sem_destroy(&barrier->mutex);
    sem_destroy(&barrier->turnstile1);
    sem_destroy(&barrier->turnstile2);
    free(barrier);
}
