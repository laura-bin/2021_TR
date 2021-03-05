/** *************************************************************************************
 * Exercice 2 : Producteurs - Consommateurs
 * ========================================
 *
 * FIFO queue access implementation:
 *  - push a new item in the FIFO queue
 *  - pop an item from the FIFO queue
 * 
 * FIFO queue creation and deletion implementation:
 *  - initialize a new FIFO queue
 *  - free a FIFO queue
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "fifo.h"

void push(char item, struct fifo *fifo) {

    // wait for a free space to be available
    sem_wait(&fifo->spaces);

    // lock access to the buffer
    sem_wait(&fifo->mutex);

    // fill the first available space in the buffer with the new item
    fifo->buffer[fifo->input] = item;

    // update next input available index
    if (fifo->input < fifo->buffer_size-1) {
        fifo->input++;
    } else {
        fifo->input = 0;
    }

    // unlock the access to the buffer
    sem_post(&fifo->mutex);

    // notify the availability of a new item
    sem_post(&fifo->items);
}

char pop(struct fifo *fifo) {
    char item;

    // wait for an item to be available
    // (avoid blocking the mutex with an empty buffer)
    sem_wait(&fifo->items);

    // lock access to the buffer
    sem_wait(&fifo->mutex);

    // get the requested value and clear the space in the buffer
    item = fifo->buffer[fifo->output];
    fifo->buffer[fifo->output] = 0;

    // update next output available index
    if (fifo->output < fifo->buffer_size-1) {
        fifo->output++;
    } else {
        fifo->output = 0;
    }

    // unlock the access to the buffer
    sem_post(&fifo->mutex);

    // notify the availability of a new free space 
    sem_post(&fifo->spaces);

    return item;
}

struct fifo *init_fifo(unsigned buffer_size) {
    struct fifo *fifo = malloc(sizeof(struct fifo));
	memset(fifo, 0, sizeof(struct fifo));

    fifo->buffer = malloc(buffer_size * sizeof(char));
	memset(fifo->buffer, 0, buffer_size * sizeof(char));

    fifo->buffer_size = buffer_size;

    // start filling the queue at index 0
    fifo->input = 0;
    fifo->output = 0;

    // mutex initialization
    sem_init(&fifo->mutex, 0, 1);

    // items count to avoid trying to get an item from an empty buffer (deadlock)
    sem_init(&fifo->items, 0, 0);

    // free spaces count to avoid trying to insert an item in a full buffer
    sem_init(&fifo->spaces, 0, buffer_size);

    return fifo;
}

void free_fifo(struct fifo *fifo) {
    free(fifo->buffer);
    sem_destroy(&fifo->mutex);
    sem_destroy(&fifo->items);
    sem_destroy(&fifo->spaces);
    free(fifo);
}
