/** *************************************************************************************
 * Lecteurs - Ecrivains
 * ====================
 * 
 * Write thread
 * Create all writers threads
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
#include "write_thread.h"
#include "reader_writer.h"

void *write_thread(void *write_thread_params) {
    struct write_params write_params;
    int end = 0;

    // copy the parameters then unlock the mutex
    struct write_thread_params params = *((struct write_thread_params *)write_thread_params);
    pthread_mutex_unlock(params.mutex);

    write_params.data = params.shared_data;
    write_params.increment_value = 1;
    write_params.writer_id = params.writer_id;

    // wait for all the threads to be created before starting to write
    pthread_barrier_wait(params.barrier);

    while (!end) {
        usleep(100);
        end = sync_write(params.rw, write_data, &write_params);
    }

    return NULL;
}

void *create_writers(void *write_thread_params) {
    int i, rv;
    struct write_thread_params *params = (struct write_thread_params *)write_thread_params;
    pthread_t *write_threads;
	pthread_attr_t attr;
	struct sched_param sched_param;

    // initialize the thread array
    write_threads = malloc(sizeof(pthread_t) * params->writers_count);
    if (write_threads == NULL) {
        perror("malloc write_threads");
        exit(1);
    }

    // initialize the threads attributes
    rv = pthread_attr_init(&attr);
    if (rv) {
        fprintf(stderr, "writer threads attr_init error: %s\n", strerror(rv));
        exit(1);
    }

    // set the scheduler to round robin
    rv = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if (rv) {
        fprintf(stderr, "writer threads setschedpolicy error: %s\n", strerror(rv));
        exit(1);
    }

    // use the new attributes and not the parent's ones
    rv = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (rv) {
        fprintf(stderr, "writer threads setinheritsched error: %s\n", strerror(rv));
        exit(1);
    }

    // set the priority
	sched_param.sched_priority = params->priority;

    // set the new parameters
    rv = pthread_attr_setschedparam(&attr, &sched_param);
    if (rv) {
        fprintf(stderr, "writer threads setschedparam error: %s\n", strerror(rv));
        exit(1);
    }

    // launch the threads
    pthread_mutex_lock(params->mutex);
    for (i = 0; i < params->writers_count; i++) {
        params->writer_id++;
        rv = pthread_create(&write_threads[i], &attr, write_thread, params);
        if (rv) {
            fprintf(stderr, "writer threads pthread_create thread %d error: %s\n",
                    params->writer_id, strerror(rv));
            exit(1);
        }

        // lock until the new thread has copied the parameters passed
        pthread_mutex_lock(params->mutex);
    }

    // wait for each thread to exit
    for (i = 0; i < params->writers_count; i++) {
        pthread_join(write_threads[i], NULL);
    }

    pthread_attr_destroy(&attr);
    free(write_threads);
    return NULL;

}
