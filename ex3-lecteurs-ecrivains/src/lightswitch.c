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
    struct lightswitch *lightswitch = malloc(sizeof(struct lightswitch));
    if (lightswitch == NULL) {
        return NULL;
    }
    memset(lightswitch, 0, sizeof(struct lightswitch));
    sem_init(&lightswitch->mutex, 0, 1);
    return lightswitch;
}

void free_lightswitch(struct lightswitch *ls) {
    sem_destroy(&ls->mutex);
    free(ls);
}

void lock_lightswitch(struct lightswitch *ls, sem_t *free_access) {
    // increment the number of threads in the critical section
    sem_wait(&ls->mutex);
    ls->thread_count += 1;

    // the first thread locks the access to the critical section
    if (ls->thread_count == 1) {
        sem_wait(free_access);
        puts("Critical section is locked by readers");
    }
    sem_post(&ls->mutex);
}

void unlock_lightswitch(struct lightswitch *ls, sem_t *free_access) {
    // decrement the number of threads in the critical section
    sem_wait(&ls->mutex);
    ls->thread_count -= 1;

    // the last thread signals the free access to the data
    if (!ls->thread_count) {
        sem_post(free_access);
        puts("Critical section is unlocked");
    }
    sem_post(&ls->mutex);
}
