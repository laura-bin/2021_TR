/** *************************************************************************************
 * Exercice 2 : Producteurs - Consommateurs
 * ========================================
 * 
 * Thread 1: reads a text file (arg1) and strores it in a first buffer
 * Thread 2: reads the first buffer, processes the data and stores it in the second buffer
 * Thread 3: reads the second buffer and displays it on the standard output
 *
 * arg[1]: text file name
 * arg[2]: buffer size
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fifo.h"
#include "prodcon.h"

int main (int argc, char *argv[]) {
    int i;
    char filename[255];
    unsigned buffer_size;
    struct read_thread_params read_params;
    struct process_thread_params process_params;
    struct display_thread_params display_params;
    pthread_t threads[THREADS_COUNT];

    // test the args
    if (argc != 3) {
        fprintf(stderr,"usage: %s filename size\n", argv[0]);
        return 1;
    }

    // get the filename
    strncpy(filename, argv[1], 255);
    if ((buffer_size = atoi(argv[2])) == -1 || buffer_size < 1
            || buffer_size > MAX_BUFFER_SIZE) {
        fprintf(stderr, "buffer size [1-%d]\n", MAX_BUFFER_SIZE);
        return 1;
    }

    // if the filename is not valid, stop the program
    read_params.fp = fopen(filename, "r");
    if (read_params.fp == NULL) {
        perror("Opening text file");
        return 1;
    }
    
    // fill the random
    srand(time(NULL));

    // fill the threads parameters
    read_params.buf1 = process_params.buf1 = init_fifo(buffer_size);
    process_params.buf2 = display_params.buf2 = init_fifo(buffer_size);

    // laounch the threads
    pthread_create(&threads[0], NULL, read_fifo, &read_params);
    pthread_create(&threads[1], NULL, process_fifo, &process_params);
    pthread_create(&threads[2], NULL, display_fifo, &display_params);

    // wait for each thread to exit before destroying the FIFO queues
    for (i = 0; i < THREADS_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // free the FIFO queues
    free_fifo(process_params.buf1);
    free_fifo(process_params.buf2);

    return 0;
}

void *read_fifo(void *read_thread_params) {
    struct read_thread_params *params = (struct read_thread_params *)read_thread_params;
    int sleep_time = rand() % 20000 + 1;
    char item;

    // read each character to add it in buf1
    while ((item = getc(params->fp)) != EOF) {
        usleep(sleep_time);
        push(item, params->buf1);
    }

    // close the text file
    fclose(params->fp);

    // keep EOF to end next theard
    push(item, params->buf1);

    return NULL;
}


void *process_fifo(void *process_thread_params) {
    struct process_thread_params *params = (struct process_thread_params *)process_thread_params;
    int sleep_time = rand() % 20000 + 1;
    char item;

    // read each character, process it then add it to buf2
    while ((item = pop(params->buf1)) != EOF) {
        usleep(sleep_time);
        push(toupper(item), params->buf2);
    }

    // keep EOF to end next theard
    push(item, params->buf2);

    return NULL;
}

void *display_fifo(void *display_thread_params) {
    struct display_thread_params *params = (struct display_thread_params *)display_thread_params;
    int sleep_time = rand() % 20000 + 1;
    char item;

    // read each character, process it then add it to buf2
    while ((item = pop(params->buf2)) != EOF) {
        usleep(sleep_time);
        printf("%c", item);
    }
    puts("");

    return NULL;
}
