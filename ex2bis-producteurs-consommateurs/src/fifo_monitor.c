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

#include "fifo_monitor.h"

void push(char item, struct fifo *fifo) {

    // lock the access to the buffer before calling pthread_cond_wait
    pthread_mutex_lock(&fifo->fifo_lock);

    // wait for a free space to be available
    while (fifo->spaces == 0) {
        pthread_cond_wait(&fifo->space_available, &fifo->fifo_lock);
    }

    // fill the first available space in the buffer with the new item
    fifo->buffer[fifo->input] = item;

    // update next input available index
    if (fifo->input < fifo->buffer_size-1) {
        fifo->input++;
    } else {
        fifo->input = 0;
    }

    // update spaces and items count
    fifo->items++;
    fifo->spaces--;

    // notify the availability of a new item
    pthread_cond_signal(&fifo->item_available);

    // unlock the buffer mutex
    pthread_mutex_unlock(&fifo->fifo_lock);
}

char pop(struct fifo *fifo) {
    char item;

    // lock access to the buffer
    pthread_mutex_lock(&fifo->fifo_lock);

    // wait for an item to be available
    // (avoid blocking the mutex with an empty buffer)
    while (fifo->items == 0) {
        pthread_cond_wait(&fifo->item_available, &fifo->fifo_lock);
    }

    // get the requested value and clear the space in the buffer
    item = fifo->buffer[fifo->output];
    fifo->buffer[fifo->output] = 0;

    // update next output available index
    if (fifo->output < fifo->buffer_size-1) {
        fifo->output++;
    } else {
        fifo->output = 0;
    }

    // update spaces and items count
    fifo->items--;
    fifo->spaces++;

    // notify the availability of a new free space 
    pthread_cond_signal(&fifo->space_available);

    // unlock the access to the buffer
    pthread_mutex_unlock(&fifo->fifo_lock);

    return item;
}

struct fifo *init_fifo(unsigned buffer_size) {
    struct fifo *fifo = malloc(sizeof(struct fifo));

    // init fifo values to 0
	memset(fifo, 0, sizeof(struct fifo));

    // except for the buffer size and the spaces count
    fifo->buffer_size = buffer_size;
    fifo->spaces = buffer_size;

    // init the buffer to 0
    fifo->buffer = malloc(buffer_size * sizeof(char));
	memset(fifo->buffer, 0, buffer_size * sizeof(char));

    // init the conditions
    pthread_cond_init(&fifo->item_available, NULL);
    pthread_cond_init(&fifo->space_available, NULL);
    
    // init the mutex
    pthread_mutex_init(&fifo->fifo_lock, NULL);

    return fifo;
}

void free_fifo(struct fifo *fifo) {
    free(fifo->buffer);
    pthread_mutex_destroy(&fifo->fifo_lock);
    pthread_cond_destroy(&fifo->item_available);
    pthread_cond_destroy(&fifo->space_available);
    free(fifo);
}
