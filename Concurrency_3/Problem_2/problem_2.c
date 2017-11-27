/*
*	Kyle Prouty
*	Nathaniel Whitlock
*
*	Operating Systems 2 - Fall 2017/2018
*	Concurrency 3 - Problem 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "mt19937ar.c"

#define THREADS 4
#define I_SLEEP 8
#define S_SLEEP 12
#define D_SLEEP 14
#define MAX 20

typedef struct item_list{
	int value;
	struct item_list *next;
}item;

void* insert_thread(void* arg);
void* search_thread(void* arg);
void* delete_thread(void* arg);
void insert(item *head, int arg);
void search(item *head, int arg);
void delete(item *head, int arg);

pthread_mutex_t a_mutex;
pthread_mutex_t b_mutex;
pthread_mutex_t c_mutex;
pthread_t a_id[THREADS];
pthread_t b_id[THREADS];
pthread_t c_id[THREADS];


int main(int argc, char **argv)
{
	item *head = calloc(1, sizeof(struct item_list));
	head->value = 0;
	head->next = NULL;

	for(int x=0; x<THREADS; x++)
		pthread_create(&a_id[x],NULL,search_thread,head);
	
	for(int x; x<THREADS; x++)
		pthread_create(&b_id[x],NULL,insert_thread,head);
	
	for(int x=0; x<THREADS; x++)
		pthread_create(&c_id[x],NULL,delete_thread,head);

	for(int x=0; x<THREADS; x++) {
		pthread_join(a_id[x], NULL);
		pthread_join(b_id[x], NULL);
		pthread_join(c_id[x], NULL);
	}
	return 0;
}

/*
*	THREADS
*/
void* search_thread(void* arg)
{
	item *head = (item *)arg;
	while(1){
		int tmp = rand()%MAX;
		
		search(head,tmp);
		printf("%s\t%d\n","S - SEARCH",tmp);
		sleep(rand()%S_SLEEP);
	}
}

void* insert_thread(void* arg)
{
	item *head = (item *)arg;
	while(1){
		int tmp = rand()%MAX;
		
		insert(head,tmp);
		printf("%s\t%d\n","I - INSERT",tmp);
		sleep(rand()%I_SLEEP);
	}
}

void* delete_thread(void* arg)
{
	item *head = (item *)arg;
	while(1){
		int tmp = rand()%MAX;

		delete(head,tmp);
		printf("%s\t%d\n","D - DELETE",tmp);
		sleep(rand()%D_SLEEP);
	}
}


/*
* 	FUNCTIONS
*/
void search(item *head, int arg)
{
	pthread_mutex_lock(&a_mutex);	
	item *current = head->next;
	while(current != NULL) {
		if(current->value == arg)
		    break;
		current = current->next;
	}
	pthread_mutex_unlock(&a_mutex);
}


void insert(item *head, int arg)
{
	pthread_mutex_lock(&b_mutex);
	item *current = head;
	while(current->next != NULL){
		current = current->next;
	}
	current->next = malloc(sizeof(item));
	current->next->value = arg;
	current->next->next = NULL;
	pthread_mutex_unlock(&b_mutex);
}


void delete(item *head, int arg)
{
	pthread_mutex_lock(&a_mutex);
	pthread_mutex_lock(&b_mutex);
	pthread_mutex_lock(&c_mutex);
	item *prevNode = head->next;
	item *nextNode;
	if(prevNode){
		nextNode = prevNode->next;
		while(nextNode && nextNode->next) {
			if(nextNode->value != arg){
				prevNode = nextNode;
				nextNode = nextNode->next;
			} else {
				prevNode->next = nextNode->next;
				break;
			}
		}
	}
	pthread_mutex_unlock(&c_mutex);
	pthread_mutex_unlock(&b_mutex);
	pthread_mutex_unlock(&a_mutex);
}