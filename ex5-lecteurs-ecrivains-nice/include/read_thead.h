#pragma once
/** *************************************************************************************
 * Lecteurs - Ecrivains
 * ====================
 * 
 * Read thread parameters structure:
 *  - read thread function prototype
 *  - create all readers threads
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

/* Read thread parameters */
struct read_thread_params {
    int reader_id;              // thread id
    int readers_count;          // number of threads
    int niceness;               // nice value
    struct data *shared_data;   // data structure shared by readers and writers
    struct sync_rw *rw;         // synchronized read/write access structure
    pthread_mutex_t *mutex;     // parameters copy mutex
};

/**
 * Calls the sync read function with the appropriate read function and parameters
 */
void *read_thread(void *read_thread_params);

/**
 * Creates all the readers threads
 */
void *create_readers(void *read_thread_params);
