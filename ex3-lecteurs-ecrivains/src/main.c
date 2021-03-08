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

    for (i = 0; i < params->readers_count; i++) {
        params->reader_id++;
        pthread_create(&read_threads[i], NULL, read_thread, params);

        // lock until the new thread has copy the race description
        sem_wait(params->params_mutex);
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

    for (i = 0; i < params->writers_count; i++) {
        params->writer_id++;
        pthread_create(&write_threads[i], NULL, write_thread, params);

        // lock until the new thread has copy the race description
        sem_wait(params->params_mutex);
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
    sem_t free_data_access;
    sem_t read_params_mutex;
    sem_t write_params_mutex;
    pthread_t reader_creator;
    pthread_t writer_creator;
    struct lightswitch *reader_ls;
    struct lightswitch *writer_ls;
    sem_t data_access_mutex;
    sem_t turnstile;
    sem_t mutex;
    sem_t no_readers_mutex;
    sem_t no_writers_mutex;

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

    sem_init(&read_params_mutex, 0, 0);
    sem_init(&write_params_mutex, 0, 0);
    read_params.params_mutex = &read_params_mutex;
    write_params.params_mutex = &write_params_mutex;
    
    read_params.reader_id = 0;
    write_params.writer_id = 0;

    read_params.data = write_params.data = init_data();

    sem_init(&free_data_access, 0, 1);
    read_params.free_access = write_params.free_access = &free_data_access;

    read_params.ls = init_lightswitch();

    read_params.read_data = read_data;
    write_params.write_data = write_data;

    sem_init(&data_access_mutex, 0, 1);
    read_params.data_access_mutex = write_params.data_access_mutex = &data_access_mutex;
 
    sem_init(&turnstile, 0, 1);
    read_params.turnstile_mutex = write_params.turnstile_mutex = &turnstile;

    reader_ls = init_lightswitch();
    read_params.read_switch = write_params.read_switch = reader_ls;

    writer_ls = init_lightswitch();
    read_params.write_switch = write_params.write_switch = writer_ls;

    sem_init(&mutex, 0, 1);
    read_params.turnstile_mutex = write_params.turnstile_mutex = &mutex;
    sem_init(&no_readers_mutex, 0, 1);
    read_params.no_readers_mutex = write_params.no_readers_mutex = &no_readers_mutex;
    sem_init(&turnstile, 0, 1);
    read_params.no_writers_mutex = write_params.no_writers_mutex = &no_writers_mutex;


     


    pthread_create(&reader_creator, NULL, create_readers, &read_params);
    pthread_create(&writer_creator, NULL, create_writers, &write_params);

    pthread_join(writer_creator, NULL);
    pthread_join(reader_creator, NULL);

    // free

    return 0;
}
