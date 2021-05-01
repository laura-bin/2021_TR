/** *************************************************************************************
 * Exercice 3 : Lecteurs - Ecrivains
 * =================================
 * 
 * Reader threads: can be in the critical section simultaneously
 * Writer threads: have exclusive access to the critical section
 * 
 *  - readers should outnumber writers
 *  - the number of readers and the number of writers must be greater than 0
 *  - data counter is initialized at 0 and max value must be greater than 0
 *
 * arg[1]: count of reader threads
 * arg[2]: readers nice value
 * arg[3]: count of writer threads
 * arg[4]: writers nice value
 * arg[5]: max data value
 * 
 * If a nice value is lower than 0, the program must be launched as sudo
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "reader_writer.h"
#include "read_thead.h"
#include "statistics.h"
#include "write_thread.h"

int main (int argc, char *argv[]) {
    int max;
    struct read_thread_params read_thread_params;
    struct write_thread_params write_thread_params;
    pthread_t reader_creator;
    pthread_t writer_creator;
    pthread_mutex_t reader_params;
    pthread_mutex_t writer_params;
    pthread_barrier_t barrier;
    int exit_code = 0;

    // test the args
    if (argc != 6) {
        fprintf(stderr, "usage: %s readers readers_niceness writers writers_niceness max_value\n", argv[0]);
        return 1;
    }

    // fill the random
    srand(time(NULL));

    // initialize the count of readers
    if ((read_thread_params.readers_count = atoi(argv[1])) <= 0) {
        exit_code = 1;
        fprintf(stderr, "invalid readers count\n");
        goto end;
    }

    // initialize readers niceness (test already implemented by the nice function)
    read_thread_params.niceness = atoi(argv[2]);
    if (read_thread_params.niceness < -20 || read_thread_params.niceness > 19) {
        exit_code = 1;
        fprintf(stderr, "invalid readers niceness\n");
        goto end;
    }

    // initialize the count of writers
    if ((write_thread_params.writers_count = atoi(argv[3])) <= 0) {
        exit_code = 1;
        fprintf(stderr, "invalid writers count\n");
        goto end;
    }

    // initialize writers niceness (test already implemented by the nice function)
    write_thread_params.niceness = atoi(argv[4]);
    if (write_thread_params.niceness < -20 || write_thread_params.niceness > 19) {
        exit_code = 1;
        fprintf(stderr, "invalid writers niceness\n");
        goto end;
    }

    // get the max data value
    if ((max = atoi(argv[5])) <= 0) {
        exit_code = 1;
        fprintf(stderr, "invalid max value\n");
        goto end;
    }

    // open the log file
    if (open_stat_file(read_thread_params.readers_count, read_thread_params.niceness,
            write_thread_params.writers_count, write_thread_params.niceness, max)) {
        fprintf(stderr, "error on stat file opening\n");
        goto end;
    }

    // initialize reader id
    read_thread_params.reader_id = 0;

    // initialize writer id
    write_thread_params.writer_id = 0;

    // initialize the shared barrier
    if (pthread_barrier_init(&barrier, NULL,
        read_thread_params.readers_count + write_thread_params.writers_count)) {
        exit_code = 1;
        fprintf(stderr, "barrier initialization");
        goto end_free_barrier;
    }
    read_thread_params.barrier = write_thread_params.barrier = &barrier;

    // initialize the shared data
    read_thread_params.shared_data = write_thread_params.shared_data = init_data(max);
    if (read_thread_params.shared_data == NULL) {
        exit_code = 1;
        perror("data initialization");
        goto end_free_data;
    }

    // initialize the shared synchronized read/write access structure
    read_thread_params.rw = write_thread_params.rw = init_rw();
    if (read_thread_params.rw == NULL) {
        exit_code = 1;
        perror("read/write access initialization");
        goto end_free_rw;
    }

    // initialize the read parameters copy mutex
    if (pthread_mutex_init(&reader_params, NULL)) {
        exit_code = 1;
        perror("reader_params mutex initialization");
        goto end_free_read_params_mutex;
    }
    read_thread_params.mutex = &reader_params;

    // initialize the write parameters copy mutex
    if (pthread_mutex_init(&writer_params, NULL)) {
        exit_code = 1;
        perror("writer_params mutex initialization");
        goto end_free_write_params_mutex;
    }
    write_thread_params.mutex = &writer_params;

    pthread_create(&reader_creator, NULL, create_readers, &read_thread_params);
    pthread_create(&writer_creator, NULL, create_writers, &write_thread_params);

    pthread_join(writer_creator, NULL);
    pthread_join(reader_creator, NULL);

    end_free_write_params_mutex:
    pthread_mutex_destroy(&writer_params);

    end_free_read_params_mutex:
    pthread_mutex_destroy(&reader_params);

    end_free_rw:
    free_rw(read_thread_params.rw);

    end_free_data:
    free_data(read_thread_params.shared_data);

    end_free_barrier:
    pthread_barrier_destroy(&barrier);
    close_stats_file();

    end:

    return exit_code;
}
