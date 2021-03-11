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

struct sync_rw *init_rw(void) {
    struct sync_rw *rw;

    rw = malloc(sizeof(struct sync_rw));
    if (rw == NULL) {
        return NULL;
    }

    if (sem_init(&rw->no_readers, 0, 1)) {
        return NULL;
    }

    if (sem_init(&rw->no_writers, 0, 1)) {
        return NULL;
    }

    rw->reader_switch = init_lightswitch();
    if (rw->reader_switch == NULL) {
        return NULL;
    }

    rw->writer_switch = init_lightswitch();
    if (rw->writer_switch == NULL) {
        return NULL;
    }

    return rw;
}

void free_rw(struct sync_rw *rw) {
    free_lightswitch(rw->writer_switch);
    free_lightswitch(rw->reader_switch);
    sem_destroy(&rw->no_writers);
    sem_destroy(&rw->no_readers);
    free(rw);
}

int sync_read(struct sync_rw *rw, int (*read_data)(void *params), void *read_params) {
    int end = 0;

    // hold the no writer semaphore
    sem_wait(&rw->no_readers);
    lock_lightswitch(rw->reader_switch, &rw->no_writers);
    sem_post(&rw->no_readers);

    // critical section: read the data
    end = read_data(read_params);

    // turn off the lightswitch (authorize the writers to update to the data)
    unlock_lightswitch(rw->reader_switch, &rw->no_writers);

    return end;
}

int sync_write(struct sync_rw *rw, int (*write_data)(void *params), void *write_params) {
    int end = 0;

    // hold the no reader & no writer semaphores
    // multiple writers can queue on no writer semaphore and readers are blocked
    lock_lightswitch(rw->writer_switch, &rw->no_readers);
    sem_wait(&rw->no_writers);

    // critical section: increment the data
    end = write_data(write_params);

    // turn off the lightswitch (authorize readers to access the data)
    sem_post(&rw->no_writers);
    unlock_lightswitch(rw->writer_switch, &rw->no_readers);

    return end;
}