#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <pthread.h>

void *block_slave(void *);

//globals
char *path;
uint64_t blocksize;
uint32_t port;
uint32_t thread_count;

void *
block_slave(void *args)
{
	printf("hello from %d\n", (uint32_t )args);
  return NULL;	
}

int 
main(int argc, char **args)
{
	if (argc < 5) {
		printf("Usage: ./fsDB <path> <blocksize> <port> <threadcount>\n");
		exit(1);
	}

	path = (char *)malloc(strlen(args[1]) *sizeof(char) + 1);
	strncpy(path, args[1], strlen(args[1]));

	char *temp;
	blocksize = strtol(args[2], &temp, 10);
	port = strtol(args[3], &temp, 10);
	thread_count = strtol(args[4], &temp, 10); 

	//This just looks like a pthreads tutorial...sigh...

	pthread_t threads[thread_count];

	for (int i = 0; i < thread_count; ++i) {
		pthread_create(&threads[i],  NULL, &block_slave, (void *)i);				
	}
  for (int i = 0; i < thread_count; ++i) {
		pthread_join(threads[i], NULL);
	}
	
	exit(0);
}
