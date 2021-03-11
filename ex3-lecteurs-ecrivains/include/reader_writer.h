#pragma once
/** *************************************************************************************
 * Exercice 3 : Lecteurs - Ecrivains
 * =================================
 * 
 * Threads parameters structures and threads functions
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include "data.h"
#include "lightswitch.h"

/* Synchronized read/write access */
struct sync_rw {
    sem_t no_readers;                   // 1 if no reader is in the critical section
    sem_t no_writers;                   // 1 if no writer is in the critical section
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
