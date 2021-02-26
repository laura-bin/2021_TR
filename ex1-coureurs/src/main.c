/** *************************************************************************************
 * Exercice 1 : coureurs
 * =====================
 *
 * Implementer en C un systeme dans lequel plusieurs threads doivent attendre
 * un meme instant avant de poursuivre leur travail. Cet instant peut se representer
 * par une barriere implementee a l’aide de semaphores.
 *
 * args: the program waits the number of runners & stages in the race
 *
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "barrier.h"

sem_t race_params_sem;          // semaphore used to udapte/read the race description

/* Race description given to the threads */
struct race {
    unsigned runner_bib;        // runner bib identifier
    unsigned stages;            // number of stages in the race
    struct barrier *barrier;    // barrier used for each stage
};

/**
 * Thread/runner function: run and wait at each stage for the others to arrive
 * 
 * @param params: race description
 */
void *just_do_it(void *params);

/**
 * Useless function showing a "start" message
 */
void start_race(void);

int main (int argc, char *argv[]) {
    int runners;                // number of runners participating in the race
    int i;                      // runner index
    pthread_t *thread;          // thread aka runner array
    struct race race;           // race description
    
    // test the args
    if (argc != 3) {
        fprintf(stderr,"usage: %s runners stages\n", argv[0]);
        return 1;
    }

    // get the number of runners & stages from the args
    runners = atoi(argv[1]);
    race.stages = atoi(argv[2]);

    // fill the random
    srand(time(NULL));

    // initialize the array of threads
    thread = malloc(sizeof(pthread_t) * runners);

    // initialize the semaphore used to pass the parameters to each thread
    sem_init(&race_params_sem, 0, 0);

    // initialize the race description
    race.barrier = init_barrier(runners);

    // start the race: create threads/runners
    start_race();
    for (i = 0; i < runners; i++) {
        race.runner_bib = i+1;
        pthread_create(&thread[i], NULL, just_do_it, &race);

        // lock until the new thread has copy the race description
        sem_wait(&race_params_sem);
    }

    // wait for each thread to exit before destroying the barrier
    for (i = 0; i < runners; i++) {
        pthread_join(thread[i], NULL);
    }

    // free the semaphore & the barrier
    free(thread);
    sem_destroy(&race_params_sem);
    free_barrier(race.barrier);

    // stop the main thread without stopping the other threads
    // pthread_exit(NULL);
}


void *just_do_it(void *params) {
    unsigned i;
    int sleep_time;

    // copy the race description then unlock the semaphore
    struct race race = *((struct race*) params);
    sem_post(&race_params_sem);

    // run each stage
    for (i = 0; i < race.stages; i++) {
        sleep_time = rand() % 3000000 + 1;
        printf(" - Runner %u starts stage %u\n", race.runner_bib, i+1);
        usleep(sleep_time);
        printf(" o Runner %u finished stage %u %10d us\n",
                race.runner_bib, i+1, sleep_time);
        barrier_wait(race.barrier);
    }

    printf(" x Runner %u finished the race\n", race.runner_bib);

    return NULL;
}

void start_race(void) {
    puts("                                o");
    puts("                               /\\");
    puts("                              /\\/\\");
    puts("                             /\\/\\/\\");
    puts("               ,a_a         /\\/\\/\\/\\");
    puts("              {/ ''\\_      /\\/\\/\\/\\/\\");
    puts("              {\\ ,_oo)    /\\/\\/\\/\\/\\/\\");
    puts("              {/  (_^____/  \\/\\/\\/\\/\\/\\");
    puts("    .=.      {/ \\___)))*)    \\/\\/\\/\\/\\/");
    puts("   (.=.`\\   {/   /=;  ~/      \\/\\/\\/\\/");
    puts("       \\ `\\{/(   \\/\\  /        \\/\\/\\/");
    puts("        \\  `. `\\  ) )           \\/\\/");
    puts("         \\    // /_/_            \\/");
    puts("          '==''---))))");
    puts(" ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    puts("                  GO !!\n");
}
