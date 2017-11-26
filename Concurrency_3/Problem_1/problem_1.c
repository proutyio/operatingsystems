/*
*	Kyle Prouty
*	Nathaniel Whitlock
*
*	Operating Systems 2 - Fall 2017/2018
*	Concurrency 3 - Problem 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include "mt19937ar.c"

#define THREADS 12

void* worker(void* arg);

pthread_t p_id[THREADS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
int shared = 0;
bool waiting = false;


int main() 
{

	for(int x=0; x<THREADS; x++)
		pthread_create(&p_id[x], NULL, worker, &x);
	
	pthread_join(p_id[0], NULL);

	return 0;		
}

void* worker(void* arg)
{
	int id = *((int *) arg);
	int w_time = 0;

	while(1) {

		sleep(rand()%5);
		if(pthread_mutex_trylock(&mutex) == 0) {

			while(shared == 3) {
				pthread_cond_wait(&cond, &mutex);
				printf("\t%s\n","FULL");
				pthread_mutex_unlock(&mutex);
			} 
			
			if(shared < 3) {
				printf("%s %i \t%s%i \t%s%i\n","SHARED:",shared,"sleep:",w_time,"id:",id);
				shared++;
				w_time = rand()%6;
				
				pthread_mutex_unlock(&mutex);
				sleep(w_time);

				pthread_mutex_lock(&mutex);
				shared--;
				if(shared == 0)
					pthread_cond_signal(&cond);
				pthread_mutex_unlock(&mutex);
			}
		}
	}
}