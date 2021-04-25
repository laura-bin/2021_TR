#pragma once
/** *************************************************************************************
 * Exercice 3 : Lecteurs - Ecrivains
 * =================================
 * 
 * Reader writer synchronization object, read and write functions
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <pthread.h>

#include "data.h"
#include "lightswitch.h"

/* Synchronized read/write access */
struct sync_rw {
    pthread_mutex_t no_readers;         // no readers in the critical section: writers are free to access it
    pthread_mutex_t no_writers;         // no writers in the critical section: readers are free to access it
    struct lightswitch *reader_switch;  // readers lightswitch (counts the readers in the critical section)
    struct lightswitch *writer_switch;  // writers lightswitch (counts the readers in the critical section)
};

/**
 * Initializes a synchronized read/write access structure
 */
struct sync_rw *init_rw(void);

/**
 * Frees a given synchronized read/write access structure
 */
void free_rw(struct sync_rw *rw);

/**
 * Synchronized reading:
 * multiple readers can access the data as long as there is no writer in the queue
 */
int sync_read(struct sync_rw *rw, int (*read_data)(void *params), void *read_params);

/**
 * Synchronized writing thread:
 * multiple writers can update the data and block the reading access
 * (as long as a writer is in the queue, no reader can access the data)
 */
int sync_write(struct sync_rw *rw, int (*write_data)(void *params), void *write_params);
