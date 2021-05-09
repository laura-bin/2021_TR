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
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#include "data.h"
#include "read_thead.h"
#include "reader_writer.h"

void *read_thread(void *read_thread_params) {
    struct read_params read_params;
    int end = 0;

    // copy the parameters then unlock the mutex
    struct read_thread_params params = *((struct read_thread_params *)read_thread_params);
    pthread_mutex_unlock(params.mutex);

    read_params.data = params.shared_data;
    read_params.reader_id = params.reader_id;

    // wait for all the threads to be created before starting to read
    pthread_barrier_wait(params.barrier);

    while (!end) {
        usleep(100);
        end = sync_read(params.rw, read_data, &read_params);
    }

    return NULL;
}

void *create_readers(void *read_thread_params) {
    int i, rv;
    struct read_thread_params *params = (struct read_thread_params *)read_thread_params;
    pthread_t *read_threads;
	pthread_attr_t attr;
	struct sched_param sched_param;

    // initialize the thread array
    read_threads = malloc(sizeof(pthread_t) * params->readers_count);
    if (read_threads == NULL) {
        perror("malloc read_threads");
        exit(1);
    }

    // initialize the threads attributes
    rv = pthread_attr_init(&attr);
    if (rv) {
        fprintf(stderr, "reader threads attr_init error: %s\n", strerror(rv));
        exit(1);
    }

    // set the scheduler to round robin
    rv = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if (rv) {
        fprintf(stderr, "reader threads setschedpolicy error: %s\n", strerror(rv));
        exit(1);
    }

    // use the new attributes and not the parent's ones
    rv = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (rv) {
        fprintf(stderr, "reader threads setinheritsched error: %s\n", strerror(rv));
        exit(1);
    }

    // set the priority
	sched_param.sched_priority = params->priority;

    // set the new parameters
    rv = pthread_attr_setschedparam(&attr, &sched_param);
    if (rv) {
        fprintf(stderr, "reader threads setschedparam error: %s\n", strerror(rv));
        exit(1);
    }

    // launch the threads
    pthread_mutex_lock(params->mutex);
    for (i = 0; i < params->readers_count; i++) {
        params->reader_id++;
        rv = pthread_create(&read_threads[i], &attr, read_thread, params);
        if (rv) {
            fprintf(stderr, "reader threads pthread_create thread %d error: %s\n",
                    params->reader_id, strerror(rv));
            exit(1);
        }

        // lock until the new thread has copied the parameters passed
        pthread_mutex_lock(params->mutex);
    }

    // wait for each thread to exit
    for (i = 0; i < params->readers_count; i++) {
        pthread_join(read_threads[i], NULL);
    }

    pthread_attr_destroy(&attr);
    free(read_threads);
    return NULL;
}
