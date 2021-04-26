#pragma once
/** *************************************************************************************
 * Lecteurs - Ecrivains
 * ====================
 * 
 * Write thread parameters structure:
 *  - write thread function prototype
 *  - create all writers threads
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

/* Write thread parameters */
struct write_thread_params {
    int writer_id;              // thread id
    int writers_count;          // number of threads
    struct data *shared_data;   // data structure shared by readers and writers
    struct sync_rw *rw;         // synchronized read/write access structure
    pthread_mutex_t *mutex;     // parameters copy mutex
};

/**
 * Calls the sync write function with the appropriate write function and parameters
 */
void *write_thread(void *write_thread_params);

/**
 * Creates all the writers threads
 */
void *create_writers(void *write_thread_params);
