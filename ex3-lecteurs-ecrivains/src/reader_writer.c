/** *************************************************************************************
 * Exercice 3 : Lecteurs - Ecrivains
 * =================================
 * 
 * Threads functions implementation
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "reader_writer.h"

void *read_thread(void *read_thread_params) {
    int sleep_time;

    // copy the parameters then unlock the mutex
    struct read_thread_params params = *((struct read_thread_params *)read_thread_params);
    pthread_mutex_unlock(params.mutex);

    sleep_time = rand() % 2000 + 1;
    usleep(sleep_time);

    // hold the no writer semaphore
    sem_wait(params.no_readers);
    lock_lightswitch(params.ls, params.no_writers);
    sem_post(params.no_readers);

    // critical section: read the data
    params.read_data(params.shared_data, params.reader_id);

    // turn off the lightswitch (authorize the writers to update to the data)
    unlock_lightswitch(params.ls, params.no_writers);

    return NULL;
}

void *write_thread(void *write_thread_params) {
    int sleep_time;

    // copy the parameters then unlock the mutex
    struct write_thread_params params = *((struct write_thread_params *)write_thread_params);
    pthread_mutex_unlock(params.mutex);
    
    sleep_time = rand() % 1000 + 1;
    usleep(sleep_time);

    // hold the no reader & no writer semaphores
    // multiple writers can queue on no writer semaphore and readers are blocked
    lock_lightswitch(params.ls, params.no_readers);
    sem_wait(params.no_writers);

    // critical section: increment the data
    params.write_data(params.shared_data, params.writer_id, params.writer_id);

    // turn off the lightswitch (authorize readers to access the data)
    sem_post(params.no_writers);
    unlock_lightswitch(params.ls, params.no_readers);

    return NULL;    
}