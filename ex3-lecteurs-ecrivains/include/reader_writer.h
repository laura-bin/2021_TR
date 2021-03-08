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

#define THREAD_COUNT 2

/* Read thread parameters */
struct read_thread_params {
    int reader_id;          // thread id
    struct data *data;      // data structure shared by all the threads
    sem_t *free_access;      // 1 if the access to the data is unlocked, 0 otherwise // critical section mutex
    struct lightswitch *ls; // 
    void (*read_data)(struct data *data, int reader_id);
    sem_t *params_mutex;
    int readers_count;
    struct lightswitch *read_switch;    // counts the readers in the critical section
    struct lightswitch *write_switch;    // counts the readers in the critical section
    sem_t *data_access_mutex;   // = roomEmpty
    sem_t *turnstile_mutex;     // turnstile for readers and mutex for writers
    sem_t *mutex;     // turnstile for readers and mutex for writers
    sem_t *no_readers_mutex;
    sem_t *no_writers_mutex;
};

/* Write thread parameters */
struct write_thread_params {
    int writer_id;          // thread id
    struct data *data;      // data structure shared by all the threads
    sem_t *free_access;      // 1 if the access to the data is unlocked, 0 otherwise
    void (*write_data)(struct data *data, int value, int writer_id);
    sem_t *params_mutex;
    int writers_count;
    struct lightswitch *read_switch;    // counts the readers in the critical section
    struct lightswitch *write_switch;    // counts the readers in the critical section
    sem_t *data_access_mutex;   // = roomEmpty
    sem_t *turnstile_mutex;     // turnstile for readers and mutex for writers
    sem_t *mutex;     // turnstile for readers and mutex for writers
    sem_t *no_readers_mutex;
    sem_t *no_writers_mutex;

};

/**
 * Reading thread:
 * 
 * @param read_thread_params
 */
void *read_thread(void *read_thread_params);

/**
 * Writing thread:
 * 
 * @param write_thread_params
 */
void *write_thread(void *write_thread_params);
