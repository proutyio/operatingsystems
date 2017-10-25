/*
*	Kyle Prouty
*	Nathaniel Whitlock
*
*	Operating Systems 2 - Fall 2017/2018
*	Concurrency Problem 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


/*
 * Philosophers:
 * - Immanuel_Kant
 * - Friedrich_Nietzsche
 * - Rene_Descartes
 * - Thomas_Aquinas 
 * - John_Locke
 */


pthread_t id[5];
pthread_mutex_t fork_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork_3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork_4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork_5 = PTHREAD_MUTEX_INITIALIZER;



void* philosopher(void *x);

void rand_wait(int min,int max);



int main() 
{
	
	return 0;
}



void* philosopher(void *x)
{
  int philosopher = *((int *) x);
  while(1) {
    /*
     * think()
     * get_forks()
     * eat()
     * put_forks()
     */
  }
	
}


