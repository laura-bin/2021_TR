#pragma once
/** *************************************************************************************
 * Exercice 2 : Producteurs - Consommateurs
 * ========================================
 *
 * Threads parameters structures & functions prototypes
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <stdio.h>

#define THREADS_COUNT 3

/* Read thread parameters structure */
struct read_thread_params {
    FILE *fp;
    struct fifo *buf1;
};

/* Process thread parameters structure */
struct process_thread_params {
    struct fifo *buf1;
    struct fifo *buf2;
};

/* Display thread parameters structure */
struct display_thread_params {
    struct fifo *buf2;
};

/**
 * Reading thread
 * reads a text file and stores in in the buf1 FIFO
 * 
 * @param params
 */
void *read_fifo(void *read_thread_params);

/**
 * Uppercase thread
 * reads the buf1 FIFO, and turn each character into uppercase
 * before filling the buf2 FIFO
 * 
 * @param params
 */
void *process_fifo(void *process_thread_params);

/**
 * Display thread
 * rreads the buf2 FIFO and display each character
 * on the standard output
 * 
 * @param params
 */
void *display_fifo(void *display_thread_params);
