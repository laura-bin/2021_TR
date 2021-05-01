/** *************************************************************************************
 * Lecteurs - Ecrivains
 * ====================
 * 
 * Read thread
 * Create all readers threads
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

#include "data.h"
#include "read_thead.h"
#include "reader_writer.h"

void *read_thread(void *read_thread_params) {
    struct read_params read_params;
    int sleep_time;
    int end = 0;

    // copy the parameters then unlock the mutex
    struct read_thread_params params = *((struct read_thread_params *)read_thread_params);
    pthread_mutex_unlock(params.mutex);

    nice(params.niceness);
    // printf("r%d nice %d\n", params.reader_id, getpriority(PRIO_PROCESS, 0));

    read_params.data = params.shared_data;
    read_params.reader_id = params.reader_id;

    // wait for all the threads to be created before starting to read
    pthread_barrier_wait(params.barrier);

    while (!end) {
        sleep_time = rand() % 2000 + 1;
        usleep(sleep_time);
        end = sync_read(params.rw, read_data, &read_params);
    }

    return NULL;
}

void *create_readers(void *read_thread_params) {
    int i;
    struct read_thread_params *params = (struct read_thread_params *)read_thread_params;
    pthread_t *read_threads;

    // initialize the thread array
    if ((read_threads = malloc(sizeof(pthread_t) * params->readers_count)) == NULL) {
        perror("malloc read_threads");
        exit(1);
    }

    pthread_mutex_lock(params->mutex);
    for (i = 0; i < params->readers_count; i++) {
        params->reader_id++;
        pthread_create(&read_threads[i], NULL, read_thread, params);

        // lock until the new thread has copied the parameters passed
        pthread_mutex_lock(params->mutex);
    }

    // wait for each thread to exit
    for (i = 0; i < params->readers_count; i++) {
        pthread_join(read_threads[i], NULL);
    }

    free(read_threads);
    return NULL;
}
