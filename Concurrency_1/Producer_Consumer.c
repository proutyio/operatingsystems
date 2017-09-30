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

#define FULL 4
#define EMPTY 0

pthread_t p_id[2];
pthread_t c_id[2];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t p_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t c_cond = PTHREAD_COND_INITIALIZER;
int items = 0;

struct Buffer {
	int item;
	int sleep;
}; 
struct Buffer buffer[FULL];

void* producer();
void* consumer();
int random_int();
void debug_print(); //REMOVE ME


int main() 
{
	pthread_create(&p_id[0], NULL, producer, NULL);
	pthread_create(&p_id[1], NULL, producer, NULL);
	pthread_create(&c_id[0], NULL, consumer, NULL);
	pthread_create(&c_id[1], NULL, consumer, NULL);

	pthread_join(c_id[0], NULL);
	pthread_join(c_id[1], NULL);
	return 0;
}


void* producer()
{	
	while(1) {
		
		if(pthread_mutex_trylock(&mutex) == 0) {
			sleep(random_int(3,7));

			while(items == FULL) {
				pthread_cond_wait(&p_cond, &mutex);
				pthread_mutex_unlock(&mutex);
			}
				buffer[items].item = random_int(0,10);
				buffer[items].sleep = random_int(2,9);
				debug_print(); //REMOVE ME
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
				printf("%s%d\n","Consuming: ",buffer[items-1].item);
				sleep(buffer[items-1].sleep); 
				items--;
				pthread_mutex_unlock(&mutex);
				pthread_cond_broadcast(&p_cond);
		}
	}
}


int random_int(int min, int max)
{
	srand(time(NULL));
	int r = rand() % (max - min) + min;
	return r;
}


void debug_print() //REMOVE ME
{
	printf("items: %d\n", items);
	printf("%s\t%d\t%d\n","Produce:",buffer[items].item, buffer[items].sleep); 
}