/*
*	Kyle Prouty
*	Nathaniel Whitlock
*
*	Operating Systems 2 - Fall 2017/2018
*	Concurrency 3 - Problem 1
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


// Consider a sharable resource with the following characteristics:

// As long as there are fewer than three processes using the resource,
//		 new processes can start using it right away.
// Once there are three processes using the resource,
//		 all three must leave before any new processes can begin using it.

// Implement a mutual exclusion solution that meets the above constraints.


// Lockable process that spawns processes.

// 

/*
	-main thread just initilizes

	-main thread spawns maker thread that can spawn worker threads

	-maker thread is inf loop, spawns workers, sleeps when resource is full

	-global resource, all threads add to it (int, pid)

	-do workers loop for inf till they lock, then do workers die after they "do work"?
	
	-

*/

#define THREADS 12

/*
*	Prototypes
*/
void* check_lock();
void* display();
void* maker();
void* worker(void* arg);
int rand_wait(int min, int max);


/*
*	Globally Shared Variables
*/
int shared_resource = 0;
int thread_count = 0;
int lock_count = 0;
pthread_t p_id[THREADS];
pthread_t m_id;
pthread_t c_id;
pthread_mutex_t a_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t b_lock = PTHREAD_MUTEX_INITIALIZER;


int main() 
{
	/*
	*	Spawn maker thread.
	*	Main thread moves execution to maker thread.
	*/
	pthread_create(&c_id, NULL, check_lock, NULL);
	pthread_create(&m_id, NULL, maker, NULL);
	pthread_join(m_id, NULL);
	return 0;		
}


void* maker() 
{

	printf("%s\n", "MAKER!");
	while(1) {
		if(thread_count < THREADS) {
			pthread_create(&p_id[thread_count], NULL, worker, &thread_count);
			thread_count++;
		}
	}

}

void* worker(void* arg)
{
	int id = *((int *) arg);


	while(1) {
		sleep(5);

		if(pthread_mutex_trylock(&a_lock) == 0) {
			lock_count++;
			printf("%s%d\n","Lock Count: ",lock_count );
			printf("%s%i\n", "WORKER!",id);
			
			sleep(5);
			lock_count--;
			//pthread_mutex_unlock(&a_lock);
			break;

		}
	}


	//printf("%i\n", thread_count );

}

void* check_lock()
{
	while(1) {
		if(lock_count == 3)
			pthread_mutex_lock(&a_lock);
		else if(lock_count == 0)
			pthread_mutex_unlock(&a_lock);
	}
}

int rand_wait(int min, int max)
{
	srand(time(NULL));
	int r = rand() % (max - min) + min;
	return r;
}
