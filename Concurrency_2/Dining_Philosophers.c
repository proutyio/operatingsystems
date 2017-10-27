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


void* immanuel_kant(void* arg);
void* friedrich_nietzsche(void* arg);
void* rene_descartes(void* arg);
void* thomas_aquinas(void* arg);
void* john_locke(void* arg);
int rand_wait(int min,int max);


int main() 
{
	pthread_create(&id[0], NULL, immanuel_kant, NULL);
	pthread_create(&id[1], NULL, friedrich_nietzsche, NULL);
	pthread_create(&id[2], NULL, rene_descartes, NULL);
	pthread_create(&id[3], NULL, thomas_aquinas, NULL);
	pthread_create(&id[4], NULL, john_locke, NULL);
	pthread_join(id[0], NULL);
	pthread_join(id[1], NULL);
	pthread_join(id[2], NULL);
	pthread_join(id[3], NULL);
	pthread_join(id[4], NULL);
	return 0;
}

void* immanuel_kant(void* arg)
{
	int think = 0;
	while(1) {
		
		if(!think){
			int x = 0;
			if(pthread_mutex_trylock(&lock) == 0) {
				x = rand_wait(1,20);
				pthread_mutex_unlock(&lock);
			}
			printf("%s%d\n","Immanuel Kant: \t\tTHINKING for ",x);
			sleep(x);
		}

		if(pthread_mutex_trylock(&fork_5) == 0) {
			if(pthread_mutex_trylock(&fork_1) == 0) {
				int x =0;
				if(pthread_mutex_trylock(&lock) == 0) {
					x = rand_wait(2,9);
					pthread_mutex_unlock(&lock);
					printf("%s%d\n","Immanuel Kant: \t\tEATING for ",x);
					sleep(x);
					think= 0;
					pthread_mutex_unlock(&fork_5);
					pthread_mutex_unlock(&fork_1);
				}
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

void* friedrich_nietzsche(void* arg)
{
	int think = 0;
	int x = 0;
	int y =0;
	while(1) {
		
		if(!think){
			if(pthread_mutex_trylock(&lock) == 0) {
				x = rand_wait(1,20);
				pthread_mutex_unlock(&lock);
			}
			printf("%s%d\n","Friedrich Nietzsche: \tTHINKING for ",x);
			sleep(x);
		}

		if(pthread_mutex_trylock(&fork_1) == 0) {
			if(pthread_mutex_trylock(&fork_2) == 0) {
				if(pthread_mutex_trylock(&lock) == 0) {
					y = rand_wait(2,9);
					pthread_mutex_unlock(&lock);
					printf("%s%d\n","Friedrich Nietzsche: \tEATING for ",y);
					sleep(y);
					think= 0;
					pthread_mutex_unlock(&fork_1);
					pthread_mutex_unlock(&fork_2);
				}
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


void* rene_descartes(void* arg)
{
	int think = 0;
	while(1) {
		
		if(!think){
			int x = 0;
			if(pthread_mutex_trylock(&lock) == 0) {
				x = rand_wait(1,20);
				pthread_mutex_unlock(&lock);
			}
			printf("%s%d\n","Rene Descartes: \tTHINKING for ",x);
			sleep(x);
		}

		if(pthread_mutex_trylock(&fork_2) == 0) {
			if(pthread_mutex_trylock(&fork_3) == 0) {
				int x =0;
				if(pthread_mutex_trylock(&lock) == 0) {
					x = rand_wait(2,9);
					pthread_mutex_unlock(&lock);
					printf("%s%d\n","Rene Descartes: \tEATING for ",x);
					sleep(x);
					think= 0;
					pthread_mutex_unlock(&fork_2);
					pthread_mutex_unlock(&fork_3);
				}
				
			} else {
				think = 1;
				pthread_mutex_unlock(&fork_2);
				pthread_mutex_unlock(&fork_3);
			}
		} else {
			think = 1;
			pthread_mutex_unlock(&fork_2);
			pthread_mutex_unlock(&fork_3);
		}
	}
}

void* thomas_aquinas(void* arg)
{
	int think = 0;
	while(1) {
		
		if(!think){
			int x = 0;
			if(pthread_mutex_trylock(&lock) == 0) {
				x = rand_wait(1,20);
				pthread_mutex_unlock(&lock);
			}
			printf("%s%d\n","Thomas Aquinas: \tTHINKING for ",x);
			sleep(x);
		}

		if(pthread_mutex_trylock(&fork_3) == 0) {
			if(pthread_mutex_trylock(&fork_4) == 0) {
				int x =0;
				if(pthread_mutex_trylock(&lock) == 0) {
					x = rand_wait(2,9);
					pthread_mutex_unlock(&lock);
					printf("%s%d\n","Thomas Aquinas: \tEATING for ",x);
					sleep(x);
					think= 0;
					pthread_mutex_unlock(&fork_3);
					pthread_mutex_unlock(&fork_4);
				}
			} else {
				think = 1;
				pthread_mutex_unlock(&fork_3);
				pthread_mutex_unlock(&fork_4);
			}
		} else {
			think = 1;
			pthread_mutex_unlock(&fork_3);
			pthread_mutex_unlock(&fork_4);
		}
	}
}

void* john_locke(void* arg)
{
	int think = 0;
	while(1) {
		
		if(!think){
			int x = 0;
			if(pthread_mutex_trylock(&lock) == 0) {
				x = rand_wait(1,20);
				pthread_mutex_unlock(&lock);
			}
			printf("%s%d\n","John Locke: \t\tTHINKING for ",x);
			sleep(x);
		}

		if(pthread_mutex_trylock(&fork_4) == 0) {
			if(pthread_mutex_trylock(&fork_5) == 0) {
				int x =0;
				if(pthread_mutex_trylock(&lock) == 0) {
					x = rand_wait(2,9);
					pthread_mutex_unlock(&lock);
					printf("%s%d\n","John Locke: \t\tEATING for ",x);
					sleep(x);
					think= 0;
					pthread_mutex_unlock(&fork_4);
					pthread_mutex_unlock(&fork_5);
				}
			} else {
				think = 1;
				pthread_mutex_unlock(&fork_4);
				pthread_mutex_unlock(&fork_5);
			}
		} else {
			think = 1;
			pthread_mutex_unlock(&fork_4);
			pthread_mutex_unlock(&fork_5);
		}
	}
}

int rand_wait(int min, int max)
{
	srand(time(NULL));
	int r = rand() % (max - min) + min;
	return r;
}