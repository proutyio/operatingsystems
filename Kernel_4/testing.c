#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#include <stdlib.h>

#define free_memory syscall(359)
#define used_memory syscall(360)

#define MB (1024 * 1024)

int main()
{
  int i;
  char *p;
  float frag_ratio = 0;
  float total_memory = 0;
  
  printf("\nBeginning test cycle...\n");
        
  total_memory = ((float)used_memory + (float)free_memory);
  frag_ratio = ((float)used_memory / total_memory);
  printf("Base fragmentation ratio: %f\nFree memory: %f\nUsed memory: %f\nTotal memory: %f\n\n",
	 frag_ratio, (float)used_memory, (float)free_memory, total_memory);
  
  /* Allocate stages of memory, populate and free */
  p = malloc(MB * 50);
  memset(p, 'A', MB * 50);
  total_memory = ((float)used_memory + (float)free_memory);
  frag_ratio = ((float)used_memory / total_memory);
  printf("50 MB alloc fragmentation ratio: %f\nFree memory: %f\nUsed memory: %f\nTotal memory: %f\n\n",
	 frag_ratio, (float)used_memory, (float)free_memory, total_memory);
  free(p);
  sleep(2)
    
  p = malloc(MB * 100);
  memset(p, 'B', MB * 100);
  total_memory = ((float)used_memory + (float)free_memory);
  frag_ratio = ((float)used_memory / total_memory);
  printf("100 MB alloc fragmentation ratio: %f\nFree memory: %f\nUsed memory: %f\nTotal memory: %f\n\n",
	 frag_ratio, (float)used_memory, (float)free_memory, total_memory);
  free(p);
  sleep(2);
  
  p = malloc(MB * 200);
  memset(p, 'C', MB * 200);
  total_memory = ((float)used_memory + (float)free_memory);
  frag_ratio = ((float)used_memory / total_memory);
  printf("200 MB alloc fragmentation ratio: %f\nFree memory: %f\nUsed memory: %f\nTotal memory: %f\n\n",
	 frag_ratio, (float)used_memory, (float)free_memory, total_memory);
  free(p);
  sleep(2);
  
  p = malloc(MB * 300);
  memset(p, 'D', MB * 300);
  total_memory = ((float)used_memory + (float)free_memory);
  frag_ratio = ((float)used_memory / total_memory);
  printf("300 MB alloc fragmentation ratio: %f\nFree memory: %f\nUsed memory: %f\nTotal memory: %f\n\n",
	 frag_ratio, (float)used_memory, (float)free_memory, total_memory);
  free(p);
  
  exit(0);

}
