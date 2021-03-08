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
    int i;
    int sleep_time;

    // copy the parameters then unlock the mutex
    struct read_thread_params params = *((struct read_thread_params *)read_thread_params);
    sem_post(params.params_mutex);

    sleep_time = rand() % 2000 + 1;
    usleep(sleep_time);

    // turn the lightswitch on (the first reader in locks the access to the data)
    lock_lightswitch(params.ls, params.free_access);

    // critical section: read the data
    params.read_data(params.data, params.reader_id);

    // turn off the lightswitch (the last reader out unlocks the access to the data)
    unlock_lightswitch(params.ls, params.free_access);

    return NULL;
}

void *write_thread(void *write_thread_params) {
    int i;
    int sleep_time;

    // copy the parameters then unlock the mutex
    struct write_thread_params params = *((struct write_thread_params *)write_thread_params);
    sem_post(params.params_mutex);
    
    sleep_time = rand() % 1000 + 1;
    usleep(sleep_time);

    // wait for the critical section to be empty
    sem_wait(params.free_access);
    // puts("Critical section is locked by a writer");

    // critical section: increment the data
    params.write_data(params.data, params.writer_id, params.writer_id);

    // signal the critical section is empty
    sem_post(params.free_access);
    // puts("Critical section is free");

    return NULL;    
}