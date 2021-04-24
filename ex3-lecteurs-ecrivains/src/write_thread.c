/** *************************************************************************************
 * Exercice 3 : Lecteurs - Ecrivains
 * =================================
 * 
 * Write thread
 * Create all writers threads
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "data.h"
#include "write_thread.h"
#include "reader_writer.h"

void *write_thread(void *write_thread_params) {
    struct write_params write_params;
    int sleep_time;
    int end = 0;

    // copy the parameters then unlock the mutex
    struct write_thread_params params = *((struct write_thread_params *)write_thread_params);
    pthread_mutex_unlock(params.mutex);

    write_params.data = params.shared_data;
    write_params.increment_value = params.writer_id;
    write_params.writer_id = params.writer_id;

    while (!end) {
        sleep_time = rand() % 2000 + 1;
        usleep(sleep_time);
        end = sync_write(params.rw, write_data, &write_params);
    }

    return NULL;
}

void *create_writers(void *write_thread_params) {
    int i;
    pthread_t *write_threads;
    struct write_thread_params *params = (struct write_thread_params *)write_thread_params;

    // initialize the thread array
    if ((write_threads = malloc(sizeof(pthread_t) * params->writers_count)) == NULL) {
        perror("malloc write_threads");
        exit(1);
    }

    pthread_mutex_lock(params->mutex);
    for (i = 0; i < params->writers_count; i++) {
        params->writer_id++;
        pthread_create(&write_threads[i], NULL, write_thread, params);

        // lock until the new thread has copied the parameters passed
        pthread_mutex_lock(params->mutex);
    }

    // wait for each thread to exit
    for (i = 0; i < params->writers_count; i++) {
        pthread_join(write_threads[i], NULL);
    }

    free(write_threads);
    return NULL;
}
