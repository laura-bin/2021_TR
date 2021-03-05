#pragma once
/** *************************************************************************************
 * Exercice 2 : Producteurs - Consommateurs
 * ========================================
 *
 * FIFO queue structure
 * 
 * FIFO queue access functions prototypes:
 *  - push a new item in the FIFO queue
 *  - pop an item from the FIFO queue
 * 
 * FIFO queue creation and deletion:
 *  - initialize a new FIFO queue
 *  - free a FIFO queue
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <semaphore.h>

#define BUFFER_SIZE 10

/* FIFO queue structure */
struct fifo {
    char *buffer;               // circular buffer
    unsigned buffer_size;       // circular buffer size
    unsigned input;             // next available input index
    unsigned output;            // first available output index
    sem_t mutex;                // enforces exclusice access to the buffer
    sem_t items;                // items stored in the buffer
    sem_t spaces;               // free spaces in the buffer
};

/**
 * Pushes a new item into the FIFO queue
 * blocks the thread in no free space is available
 * 
 * @param item: new item to push in the queue
 * @param fifo: queue in which push the new item
 */
void push(char item, struct fifo *fifo);

/**
 * Pops the first available item from the FIFO queue
 * blocks the thread if no item is available
 * 
 * @param fifo: FIFO queue from which pop the element
 * 
 * @return the element poped
 */
char pop(struct fifo *fifo);

/**
 * Initializes a FIFO queue structure
 * 
 * @param buffer_size: number of elements in the circular buffer
 * 
 * @return the new FIFO queue pointer
 */
struct fifo *init_fifo(unsigned buffer_size);

/**
 * Frees a FIFO queue structure
 * 
 * @param fifo: FIFO queue to free
 */
void free_fifo(struct fifo *fifo);
