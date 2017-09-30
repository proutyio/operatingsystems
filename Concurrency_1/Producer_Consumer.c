/*
*	Kyle Prouty
*	Nathaniel Whitlock
*
*	Operating Systems 2 - Fall 2017/2018
*	Concurrency Problem 1		
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define FULL 32
#define EMPTY 0
#define THREADS 10

pthread_t p_id[THREADS];
pthread_t c_id[THREADS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t p_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t c_cond = PTHREAD_COND_INITIALIZER;
int items = 0;

struct Buffer {
	int item;
	int sleep;
}; 
struct Buffer buffer[FULL];

void* producer(void *x);
void* consumer();
int random_int();
void debug_print(int x); //REMOVE ME


int main() 
{
	int current_producer[THREADS];

	for(int i=0; i<THREADS; i++) {
		current_producer[i] = i;
		pthread_create(&p_id[i], NULL, producer, &current_producer[i]);
		pthread_create(&c_id[i], NULL, consumer, NULL);
	}

	for(int i=0; i<THREADS; i++)
		pthread_join(c_id[i], NULL);

	return 0;
}


void* producer(void *x)
{	
	int current_producer = *((int *) x);
	
	while(1) {
		if(pthread_mutex_trylock(&mutex) == 0) {
			sleep(random_int(3,7));

			while(items == FULL) {
				pthread_cond_wait(&p_cond, &mutex);
				pthread_mutex_unlock(&mutex);
			}
			buffer[items].item = random_int(0,10);
			buffer[items].sleep = random_int(2,9);
			
			debug_print(current_producer); //REMOVE ME
			
			items++;
			pthread_mutex_unlock(&mutex);
			pthread_cond_broadcast(&c_cond);
		}
	}
}


void* consumer()
{
	while(1) {
		if(pthread_mutex_trylock(&mutex) == 0) {
			while(items == EMPTY) {
				pthread_cond_wait(&c_cond, &mutex);
				pthread_mutex_unlock(&mutex);
			} 
			sleep(buffer[items-1].sleep); 
			printf("%s%d\n","Consuming: ",buffer[items-1].item);
			items--;
			pthread_cond_broadcast(&p_cond);
			pthread_mutex_unlock(&mutex);
		}
	}
}


int random_int(int min, int max)
{
	srand(time(NULL));
	int r = rand() % (max - min) + min;
	return r;
}


void debug_print(int x) //REMOVE ME
{
	printf("buffer: %d\n", items);
	printf("%s%i%s\t%d\t%d\n","Producer ",x,":",buffer[items].item, buffer[items].sleep); 
}