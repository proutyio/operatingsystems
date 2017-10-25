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


struct holding {
	int left_fork;
	int right_fork;
};
//struct holding forks[4];



void* philosopher(void* arg);
int rand_wait(int min,int max);



int main() 
{
	struct holding forks[4];

	forks[0].left_fork=5;
	forks[0].right_fork=1;	

	philosopher((void*)&forks[0]);

	
	return 0;
}



void* philosopher(void* args)
{
	struct holding *forks = args;

	printf("%i\n", forks->left_fork);
	printf("%i\n", forks->right_fork);
}



int rand_wait(int min, int max)
{
	srand(time(NULL));
	int r = rand() % (max - min) + min;
	return r;
}


