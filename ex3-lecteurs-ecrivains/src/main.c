/** *************************************************************************************
 * Exercice 3 : Lecteurs - Ecrivains
 * =================================
 * 
 * Reader threads: can be in teh critical section simultaneously
 * Writer threads: have exclusive and priority access to the critical section
 * Readers must outnumber writers
 * Implementation without threads priorities
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

        // lock until the new thread has copy the race description
        pthread_mutex_lock(params->mutex);
    }

    // wait for each thread to exit before destroying the FIFO queues
    for (i = 0; i < params->readers_count; i++) {
        pthread_join(read_threads[i], NULL);
    }
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

        // lock until the new thread has copy the race description
        pthread_mutex_lock(params->mutex);
    }

    // wait for each thread to exit before destroying the FIFO queues
    for (i = 0; i < params->writers_count; i++) {
        pthread_join(write_threads[i], NULL);
    }

    return NULL;
}

int main (int argc, char *argv[]) {
    struct read_thread_params read_params;
    struct write_thread_params write_params;
    pthread_t reader_creator;
    pthread_t writer_creator;
    pthread_mutex_t reader_params;
    pthread_mutex_t writer_params;
    sem_t no_readers;
    sem_t no_writers;

    // test the args
    if (argc != 3) {
        fprintf(stderr, "usage: %s readers writers\n", argv[0]);
        return 1;
    }
    
    // fill the random
    srand(time(NULL));

    // initialize the count of readers
    if ((read_params.readers_count = atoi(argv[1])) <= 0) {
        fprintf(stderr, "invalid readers count\n");
    }

    // initialize the count of writers
    if ((write_params.writers_count = atoi(argv[2])) <= 0) {
        fprintf(stderr, "invalid writers count\n");
    }

    // initialize reader id
    read_params.reader_id = 0;

    // initialize writer id
    write_params.writer_id = 0;

    // initialize the shared data
    read_params.shared_data = write_params.shared_data = init_data();
    if (read_params.shared_data == NULL) {
        perror("data initialization");
        return 1;
    }

    // initialize the read function
    read_params.read_data = read_data;

    // initialize the write function
    write_params.write_data = write_data;

    // initialize the shared readers turnstile
    sem_init(&no_readers, 0, 1);
    read_params.no_readers = write_params.no_readers = &no_readers;

    // initialize the shared writers turnstile
    sem_init(&no_writers, 0, 1);
    read_params.no_writers = write_params.no_writers = &no_writers;

    // initialize the readers lightswitch
    read_params.ls = init_lightswitch();
    if (read_params.ls == NULL) {
        perror("readers lightswitch initialization");
    }

    // initialize the writers lightswitch
    write_params.ls = init_lightswitch();
    if (write_params.ls == NULL) {
        perror("writers lightswitch initialization");
    }

    // initialize the read parameters copy mutex
    pthread_mutex_init(&reader_params, NULL);
    read_params.mutex = &reader_params;

    // initialize the write parameters copy mutex
    pthread_mutex_init(&writer_params, NULL);
    write_params.mutex = &writer_params;

    pthread_create(&reader_creator, NULL, create_readers, &read_params);
    pthread_create(&writer_creator, NULL, create_writers, &write_params);

    pthread_join(writer_creator, NULL);
    pthread_join(reader_creator, NULL);

    // free

    return 0;
}
