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

/* Read thread parameters */
struct read_thread_params {
    int reader_id;              // thread id
    int readers_count;          // number of threads
    struct data *shared_data;   // data structure shared by readers and writers
    int (*read_data)(struct data *data, int reader_id);
    sem_t *no_readers;          // 1 if no reader is in the critical section (shared)
    sem_t *no_writers;          // 1 if no writer is in the critical section (shared)
    struct lightswitch *ls;     // readers lightswitch (counts the readers in the critical section)
    pthread_mutex_t *mutex;     // parameters copy mutex
};

/* Write thread parameters */
struct write_thread_params {
    int writer_id;              // thread id
    int writers_count;          // number of threads
    struct data *shared_data;   // data structure shared by readers and writers
    int (*write_data)(struct data *data, int value, int writer_id);
    sem_t *no_readers;          // 1 if no reader is in the critical section (shared)
    sem_t *no_writers;          // 1 if no writer is in the critical section (shared)
    struct lightswitch *ls;     // writers lightswitch (counts the writers in the critical section)
    pthread_mutex_t *mutex;     // parameters copy mutex
};

/**
 * Reading thread:
 * multiple readers can access the data as long as there is no writer in the queue
 * 
 * @param read_thread_params
 */
void *read_thread(void *read_thread_params);

/**
 * Writing thread:
 * multiple writers can update the data and block the reading access
 * (as long as a writer is in the queue, no reader can access the data)
 * 
 * @param write_thread_params
 */
void *write_thread(void *write_thread_params);
