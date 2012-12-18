#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <pthread.h>
#include "thread_pool.h"
#include "buffer.h"

void *pool_func(void *);

//globals
char *path;
uint64_t blocksize;
uint32_t port;
uint32_t thread_count;

thread_pool pool;
buffer access_buffer;

void *
pool_func(void *args)
{
	printf("hello from %d\n", (int32_t)((int64_t)args));
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

	init_pool(&pool, thread_count, &pool_func);
	if (init_buffer(&access_buffer, blocksize, 100, 1000)) {
			
	} else {
		printf("Access buffer failed to init.");
		exit(1);
	}


	start_pool(&pool);
	wait_pool(&pool);
	free_pool(&pool);
	
	exit(0);
}
