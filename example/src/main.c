#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define NB_THREADS 5

void *fn_thread (void *numero);

static int compteur = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main (void) {
    pthread_t thread[NB_THREADS];
    int i;
    int ret;

    for (i = 0; i < NB_THREADS; i++) {
        if ((ret = pthread_create(& thread[i], NULL,
                    fn_thread, (void *) i)) != 0) {
            fprintf(stderr, "%s", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    while (1) {
        pthread_mutex_lock(&mutex);
        if (compteur >= 40) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        fprintf(stdout, "main : compteur = %d \n", compteur);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }

    for (i = 0; i < NB_THREADS; i++) {
        pthread_join(thread[i], NULL);
        return EXIT_SUCCESS;
    }
}

void *fn_thread (void *num) {
    int numero = (int) num + 1;

    while (1) {
        usleep(numero * 100000);

        pthread_mutex_lock(&mutex);
        compteur ++;
        fprintf(stdout, "Thread %d : compteur = %d \n", numero, compteur);
        if (compteur >= 40) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(NULL);
}
