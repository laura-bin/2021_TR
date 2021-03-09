#pragma once
/** *************************************************************************************
 * Exercice 3 : Lecteurs - Ecrivains
 * =================================
 * 
 * Lightswitch structure:
 *  - creation and deletion
 *  - lock and unlock
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <pthread.h>
#include <semaphore.h>

/* Lightswitch structure */
struct lightswitch {
    int counter;                    // number of threads in the critical section
    pthread_mutex_t counter_mutex;  // thread counter mutex
};

/**
 * Initializes a new lightswitch
 * 
 * @return either
 *      the lightswitch structure created
 *      NULL if an error occured (errno is set)
 */
struct lightswitch *init_lightswitch(void);

/**
 * Frees a lightswitch structure
 * 
 * @param ligthswitch: lightswitch to free
 */
void free_lightswitch(struct lightswitch *lightswitch);

/**
 * Locks the critical section:
 * the first thread entering into the critical section locks the access
 * 
 * @param lightswitch used to keep count of the entering threads
 * @param semaphore to lock
 */
void lock_lightswitch(struct lightswitch *lightswitch, sem_t *semaphore);

/**
 * Unlocks the critical section:
 * the last thread leaving the critical section locks the access
 * 
 * @param ligthswitch used to keep count of the leaving threads
 * @param semaphore to unlock
 */
void unlock_lightswitch(struct lightswitch *lightswitch, sem_t *semaphore);
