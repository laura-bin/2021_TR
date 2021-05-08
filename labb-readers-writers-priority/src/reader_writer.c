/** *************************************************************************************
 * Lecteurs - Ecrivains
 * ====================
 * 
 * Reader-writer implementation without priority
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

    rw->lightswitch = init_lightswitch();
    if (rw->lightswitch == NULL) {
        return NULL;
    }

    if (pthread_mutex_init(&rw->data_mutex, NULL)) {
        return NULL;
    }

    if (pthread_mutex_init(&rw->turnstile, NULL)) {
        return NULL;
    }

    return rw;
}

void free_rw(struct sync_rw *rw) {
    free_lightswitch(rw->lightswitch);
    pthread_mutex_destroy(&rw->data_mutex);
    pthread_mutex_destroy(&rw->turnstile);
}

int sync_read(struct sync_rw *rw, int (*read_data)(void *params), void *read_params) {
    int end = 0;

    pthread_mutex_lock(&rw->turnstile);
    pthread_mutex_unlock(&rw->turnstile);

    // first reader entering locks the data access
    lock_lightswitch(rw->lightswitch, &rw->data_mutex);

    // critical section: read the data
    end = read_data(read_params);

    // last reader leaving unlocks the data access
    unlock_lightswitch(rw->lightswitch, &rw->data_mutex);

    return end;
}

int sync_write(struct sync_rw *rw, int (*write_data)(void *params), void *write_params) {
    int end = 0;

    pthread_mutex_lock(&rw->turnstile);

    // lock the access to the data
    pthread_mutex_lock(&rw->data_mutex);

    // critical section: increment the data
    end = write_data(write_params);

    pthread_mutex_unlock(&rw->turnstile);

    // unlock the access to the data
    pthread_mutex_unlock(&rw->data_mutex);

    return end;
}
