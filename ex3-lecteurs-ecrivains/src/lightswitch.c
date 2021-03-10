/** *************************************************************************************
 * Exercice 3 : Lecteurs - Ecrivains
 * =================================
 * 
 * Lightswitch functions implementation:
 *  - creation and deletion
 *  - lock and unlock
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lightswitch.h"

struct lightswitch *init_lightswitch(void) {
    struct lightswitch *ls;

    // initialize the lightswitch structure to 0
    ls = malloc(sizeof(struct lightswitch));
    if (ls == NULL) {
        return NULL;
    }
    memset(ls, 0, sizeof(struct lightswitch));

    // initialize the mutex with default parameters
    pthread_mutex_init(&ls->counter_mutex, NULL);

    return ls;
}

void free_lightswitch(struct lightswitch *ls) {
    pthread_mutex_destroy(&ls->counter_mutex);
    free(ls);
}

void lock_lightswitch(struct lightswitch *ls, sem_t *sem) {
    // increment the number of threads in the critical section
    pthread_mutex_lock(&ls->counter_mutex);
    ls->counter += 1;

    // the first thread locks the access to the critical section
    if (ls->counter == 1) {
        sem_wait(sem);
        // puts("lightswitch locked");
    }
    pthread_mutex_unlock(&ls->counter_mutex);
}

void unlock_lightswitch(struct lightswitch *ls, sem_t *sem) {
    // decrement the number of threads in the critical section
    pthread_mutex_lock(&ls->counter_mutex);
    ls->counter -= 1;

    // the last thread signals the free access to the data
    if (!ls->counter) {
        sem_post(sem);
        // puts("lightswitch unlocked");
    }
    pthread_mutex_unlock(&ls->counter_mutex);
}
