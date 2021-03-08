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

#include <semaphore.h>

/* Lightswitch structure */
struct lightswitch {
    int thread_count;       // number of threads in the critical section
    sem_t mutex;            // thread count mutex
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
 * @param ligthswitch
 * @param free_access
 */
void lock_lightswitch(struct lightswitch *lightswitch, sem_t *free_access);

/**
 * Unlocks the critical section:
 * the last thread leaving the critical section locks the access
 * 
 * @param ligthswitch
 * @param free_access
 */
void unlock_lightswitch(struct lightswitch *lightswitch, sem_t *free_access);
