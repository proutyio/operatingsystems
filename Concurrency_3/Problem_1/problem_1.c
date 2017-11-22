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
void* maker();
void* worker(void* arg);
int rand_wait(int min, int max);


/*
*	Globally Shared Variables
*/
int thread_count = 0;
pthread_t p_id[THREADS];
pthread_mutex_t a_lock = PTHREAD_MUTEX_INITIALIZER;


int main() 
{



	maker();
	return 0;		
}


void* maker() 
{
	printf("%s\n", "MAKER!");
	//while(1) {
		//if(thread_count <= THREADS) {
			pthread_create(&p_id[thread_count], NULL, worker, &thread_count);
			//thread_count++;
	//	}
	//}
			pthread_join(p_id[thread_count], NULL);

}

void* worker(void* arg)
{
	printf("%s\n", "WORKER!");
	//printf("%i\n", thread_count );

}

int rand_wait(int min, int max)
{
	srand(time(NULL));
	int r = rand() % (max - min) + min;
	return r;
}
