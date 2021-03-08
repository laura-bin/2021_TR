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

    // hold the no writer mutex
    sem_wait(params.no_readers_mutex);
    lock_lightswitch(params.read_switch, params.no_writers_mutex);
    sem_post(params.no_readers_mutex);

    // critical section: read the data
    params.read_data(params.data, params.reader_id);

    // turn off the lightswitch (the last reader out unlocks the access to the data)
    // unlock no writer mutex (authorize writers to enter the critical section)
    unlock_lightswitch(params.read_switch, params.no_writers_mutex);

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

    // hold the no reader mutex & no writer mutex
    // multiple writers can queue on no writer mutex and readers are blocked
    lock_lightswitch(params.write_switch, params.no_readers_mutex);
    sem_wait(params.no_writers_mutex);

    // puts("Critical section is locked by a writer");

    // critical section: increment the data
    params.write_data(params.data, params.writer_id, params.writer_id);

    unlock_lightswitch(params.write_switch, params.no_readers_mutex);
    // puts("Critical section is free");

    return NULL;    
}