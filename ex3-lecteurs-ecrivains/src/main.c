/** *************************************************************************************
 * Exercice 3 : Lecteurs - Ecrivains
 * =================================
 * 
 * Reader threads: can be in the critical section simultaneously
 * Writer threads: have exclusive and priority access to the critical section
 * 
 *  - readers should outnumber writers
 *  - the number of readers and the number of writers must be greater than 0
 *  - data counter is initialized at 0 and max value must be greater than 0
 *
 * arg[1]: count of reader threads
 * arg[2]: count of writer threads
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "reader_writer.h"

void *create_readers(void *read_params) {
    int i;
    struct read_thread_params *params = (struct read_thread_params *)read_params;
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

void *create_writers(void *write_params) {
    int i;
    pthread_t *write_threads;
    struct write_thread_params *params = (struct write_thread_params *)write_params;

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

int main (int argc, char *argv[]) {
    int max;
    struct read_thread_params read_params;
    struct write_thread_params write_params;
    pthread_t reader_creator;
    pthread_t writer_creator;
    pthread_mutex_t reader_params;
    pthread_mutex_t writer_params;
    sem_t no_readers;
    sem_t no_writers;
    int exit_code = 0;

    // test the args
    if (argc != 4) {
        fprintf(stderr, "usage: %s readers writers maxvalue\n", argv[0]);
        return 1;
    }

    // fill the random
    srand(time(NULL));

    // initialize the count of readers
    if ((read_params.readers_count = atoi(argv[1])) <= 0) {
        exit_code = 1;
        fprintf(stderr, "invalid readers count\n");
        goto end;
    }

    // initialize the count of writers
    if ((write_params.writers_count = atoi(argv[2])) <= 0) {
        exit_code = 1;
        fprintf(stderr, "invalid writers count\n");
        goto end;
    }

    // get the max data value
    if ((max = atoi(argv[3])) <= 0) {
        exit_code = 1;
        fprintf(stderr, "invalid max value\n");
        goto end;
    }

    // initialize reader id
    read_params.reader_id = 0;

    // initialize writer id
    write_params.writer_id = 0;

    // initialize the shared data
    read_params.shared_data = write_params.shared_data = init_data(max);
    if (read_params.shared_data == NULL) {
        exit_code = 1;
        perror("data initialization");
        goto end_free_data;
    }

    // initialize the read function
    read_params.read_data = read_data;

    // initialize the write function
    write_params.write_data = write_data;

    // initialize the shared readers turnstile
    if (sem_init(&no_readers, 0, 1)) {
        exit_code = 1;
        perror("no_readers initialization");
        goto end_free_no_readers;
    }
    read_params.no_readers = write_params.no_readers = &no_readers;

    // initialize the shared writers turnstile
    if (sem_init(&no_writers, 0, 1)) {
        exit_code = 1;
        perror("no_writers initialization");
        goto end_free_no_writers;
    }
    read_params.no_writers = write_params.no_writers = &no_writers;

    // initialize the readers lightswitch
    read_params.ls = init_lightswitch();
    if (read_params.ls == NULL) {
        exit_code = 1;
        perror("readers lightswitch initialization");
        goto end_free_readers_ls;
    }

    // initialize the writers lightswitch
    write_params.ls = init_lightswitch();
    if (write_params.ls == NULL) {
        exit_code = 1;
        perror("writers lightswitch initialization");
        goto end_free_writers_ls;
    }

    // initialize the read parameters copy mutex
    if (pthread_mutex_init(&reader_params, NULL)) {
        exit_code = 1;
        perror("reader_params mutex initialization");
        goto end_free_read_params_mutex;
    }
    read_params.mutex = &reader_params;

    // initialize the write parameters copy mutex
    if (pthread_mutex_init(&writer_params, NULL)) {
        exit_code = 1;
        perror("writer_params mutex initialization");
        goto end_free_write_params_mutex;
    }
    write_params.mutex = &writer_params;

    pthread_create(&reader_creator, NULL, create_readers, &read_params);
    pthread_create(&writer_creator, NULL, create_writers, &write_params);

    pthread_join(writer_creator, NULL);
    pthread_join(reader_creator, NULL);

    end_free_write_params_mutex:
    pthread_mutex_destroy(&writer_params);

    end_free_read_params_mutex:
    pthread_mutex_destroy(&reader_params);

    end_free_writers_ls:
    free_lightswitch(write_params.ls);

    end_free_readers_ls:
    free_lightswitch(read_params.ls);

    end_free_no_writers:
    sem_destroy(&no_writers);

    end_free_no_readers:
    sem_destroy(&no_readers);

    end_free_data:
    free_data(read_params.shared_data);

    end:

    return exit_code;
}

