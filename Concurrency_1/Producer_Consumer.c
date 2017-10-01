/*
*	Kyle Prouty
*	Nathaniel Whitlock
*
*	Operating Systems 2 - Fall 2017/2018
*	Concurrency Problem 1
*	-- Extra Credit --	
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <cpuid.h>
#include "mt19937ar.c"

#define THREADS 10
#define FULL 32
#define EMPTY 0

pthread_t p_id[THREADS];
pthread_t c_id[THREADS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t p_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t c_cond = PTHREAD_COND_INITIALIZER;
int items = 0;
int check = 0;

struct Buffer {
	int item;
	int sleep;
}; 
struct Buffer buffer[FULL];

void* producer(void *x);
void* consumer(void *x);
int check_rdrand();
int random_int();
int rdrand();


int main() 
{
	check_rdrand();
	 
	int p[THREADS];
	int c[THREADS];
	for(int i=0; i<THREADS; i++) {
		p[i] = i;
		c[i] = i;
		pthread_create(&p_id[i], NULL, producer, &p[i]);
		pthread_create(&c_id[i], NULL, consumer, &c[i]);
	}
	for(int i=0; i<THREADS; i++)
		pthread_join(c_id[i], NULL);
	return 0;
}

void* producer(void *x)
{	
	int producer = *((int *) x);
	while(1) {
		if(pthread_mutex_trylock(&mutex) == 0) {
			sleep(random_int(3,7));
			while(items == FULL) {
				pthread_cond_wait(&p_cond, &mutex);
				pthread_mutex_unlock(&mutex);
			}
			buffer[items].item = random_int(0,10);
			buffer[items].sleep = random_int(2,9);
			printf("%s%i\n%s%i%s\t%d\t%d\n","buffer:\t",items,
				"(",producer,")Produced:",buffer[items].item, buffer[items].sleep); 
			items++;
			pthread_mutex_unlock(&mutex);
			pthread_cond_broadcast(&c_cond);
		}
	}
}

void* consumer(void *x)
{
	int consumer = *((int *) x);
	while(1) {
		if(pthread_mutex_trylock(&mutex) == 0) {
			while(items == EMPTY) {
				pthread_cond_wait(&c_cond, &mutex);
				pthread_mutex_unlock(&mutex);
			} 
			sleep(buffer[items-1].sleep); 
			printf("%s%i%s\t%d\n","(",consumer,")Consumed:",buffer[items-1].item);
			items--;
			pthread_cond_broadcast(&p_cond);
			pthread_mutex_unlock(&mutex);
		}
	}
}

int check_rdrand()
{
    unsigned int a,b,c,d, reg = (1 << 30);
    __cpuid(1, a, b, c, d);
    if( ((c & reg) == reg) == 1)
    	check = 1;
    else 
    	check = 0;
}

int random_int(int min, int max)
{
	int value = 0;
	if(check == 1) {
		value = abs(rdrand() % ((max+1) - min)) + min;
	} else {
		value = (genrand_int32() % ((max-3) + min)) + min;
	}
	return value;
}

int rdrand()
{
	unsigned int x;
	__asm__("rdrand  %[x]": [x] "=r" (x) :: "cc");
	return x;	
}