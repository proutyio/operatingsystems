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

pthread_t p_id;
pthread_t c_id;
pthread_mutex_t m_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty_cond = PTHREAD_COND_INITIALIZER;
int item_count = 0;

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
	pthread_create(&p_id, NULL, producer, NULL);
	pthread_create(&c_id, NULL, consumer, NULL);
	pthread_join(p_id, NULL);
	pthread_join(c_id, NULL);
	return 0;
}


void* producer()
{	
	while(1) {
		if(item_count == FULL) {
			pthread_cond_wait(&full_cond, &m_lock);
		} else {
			pthread_mutex_lock(&m_lock);
			sleep(random_int(3,7));
			buffer[item_count].item = random_int(0,10);
			buffer[item_count].sleep = random_int(2,9);
			
			debug_print(); //REMOVE ME
			
			item_count++;
			pthread_cond_signal(&empty_cond);
			pthread_mutex_unlock(&m_lock);
		}
	}
}


void* consumer()
{
	while(1) {
		if(item_count == EMPTY) {
			pthread_cond_wait(&empty_cond, &m_lock);
		} else {
			printf("%s%d\n","Consuming: ",buffer[item_count-1].item);
			sleep(buffer[item_count-1].sleep); 
			item_count--;
			pthread_mutex_unlock(&m_lock);
			pthread_cond_signal(&full_cond);
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
	printf("item_count: %d\n", item_count);
	printf("%s\t%d\t%d\n","Produce:",buffer[item_count].item, buffer[item_count].sleep); 
}