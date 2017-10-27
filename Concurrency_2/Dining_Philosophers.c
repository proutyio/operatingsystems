/*
*	Kyle Prouty
*	Nathaniel Whitlock
*
*	Operating Systems 2 - Fall 2017/2018
*	Concurrency Problem 2
*/

/*
 * Philosophers:
 * - Immanuel_Kant
 * - Friedrich_Nietzsche
 * - Rene_Descartes
 * - Thomas_Aquinas 
 * - John_Locke
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_t id[5];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork_3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork_4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fork_5 = PTHREAD_MUTEX_INITIALIZER;


void* philosopher(void* arg);
void* philosopher2(void* arg);
int rand_wait(int min,int max);


int main() 
{
	

	pthread_create(&id[0], NULL, philosopher, NULL);
	pthread_create(&id[1], NULL, philosopher2, NULL);
	pthread_join(id[0], NULL);
	pthread_join(id[1], NULL);
	return 0;
}


void* philosopher(void* args)
{
	int think = 0;
	while(1) {
		
		if(!think){
			int x = 0;
			if(pthread_mutex_trylock(&lock) == 0) {
				x = rand_wait(1,20);
				pthread_mutex_unlock(&lock);
			}
			printf("Immanuel_Kant: Thinking\n");
			sleep(x);
		}

		if(pthread_mutex_trylock(&fork_5) == 0) {
			if(pthread_mutex_trylock(&fork_1) == 0) {
				printf("%s\n","Immanuel_Kant: Eating\n");
				sleep(rand_wait(2,9));
				think= 0;
				pthread_mutex_unlock(&fork_5);
				pthread_mutex_unlock(&fork_1);
			} else {
				think = 1;
				pthread_mutex_unlock(&fork_5);
				pthread_mutex_unlock(&fork_1);
			}
		} else {
			think = 1;
			pthread_mutex_unlock(&fork_5);
			pthread_mutex_unlock(&fork_1);
		}
	}
}

void* philosopher2(void* args)
{
	int think = 0;
	while(1) {

		if(!think){
			int x = 0;
			if(pthread_mutex_trylock(&lock) == 0) {
				x = rand_wait(1,20);
				pthread_mutex_unlock(&lock);
			}
			printf("Friedrich_Nietzsche: Thinking\n");
			sleep(x);
		}

		if(pthread_mutex_trylock(&fork_1) == 0) {
			if(pthread_mutex_trylock(&fork_2) == 0) {
				printf("%s\n","Friedrich_Nietzsche: Eating\n");
				sleep(rand_wait(2,9));
				think= 0;
				pthread_mutex_unlock(&fork_1);
				pthread_mutex_unlock(&fork_2);
			} else {
				think = 1;
				pthread_mutex_unlock(&fork_1);
				pthread_mutex_unlock(&fork_2);
			}
		} else {
			think = 1;
			pthread_mutex_unlock(&fork_1);
			pthread_mutex_unlock(&fork_2);
		}
	}
}

int rand_wait(int min, int max)
{
	srand(time(NULL));
	int r = rand() % (max - min) + min;
	return r;
}