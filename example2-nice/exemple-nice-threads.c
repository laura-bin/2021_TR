// ---------------------------------------------------------------------
// exemple-nice-threads.c
// Fichier d'exemple du livre "Solutions Temps-Reel sous Linux"
// (C) 2012 - Christophe BLAESS
// http://christophe.blaess.fr
// ---------------------------------------------------------------------

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


void * fonction_thread (void * arg)
{
	int nice_value = (int) arg;
	time_t debut;
	long int nb_iterations  = 0;

	nice(nice_value);
	debut = time(NULL);

	while (time(NULL) < (debut + 10))
		nb_iterations ++;
	
	return (void *) nb_iterations;
}

#define NB_THREADS  5

int main(void)
{
	int i;
	void * resultat;
	int nice_value[NB_THREADS];
	pthread_t thread[NB_THREADS];
	long int  total_iterations = 0;
	long int  nb_iterations[NB_THREADS];

	for (i = 0; i < NB_THREADS; i ++)
		nice_value[i] = (i+1) * (20 / NB_THREADS); 

	for (i = 0; i < NB_THREADS; i ++)
		pthread_create(& (thread[i]), NULL, fonction_thread, 
		               (void *) nice_value[i]);
		               
	for (i = 0; i < NB_THREADS; i ++) {
		pthread_join(thread[i], & resultat);
		nb_iterations[i] = (long int) resultat;
		total_iterations += nb_iterations[i];
	}
	for (i = 0; i < NB_THREADS; i ++)
		printf("[%d] %ld (%.0f)\n", nice_value[i],
		       nb_iterations[i],
		       100.0 * nb_iterations[i] / total_iterations);
	return EXIT_SUCCESS;
}
