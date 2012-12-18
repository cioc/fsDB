#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include "producer_consumer.h"

producer_consumer prod_cons;

void *
echo_listener(void *data)
{
  printf("LISTENER\n");
  return NULL;
}

void *
echo_handler(void *data)
{
  printf("HANDLER\n");
  return NULL;
}

int 
main(int argc, char **args)
{
  printf("STARTING ECHO SERVER\n");
  init_producer_consumer( &prod_cons, 
                          1, 
                          5, 
                          sizeof(int32_t),
                          echo_listener,
                          echo_handler);  
  start_producer_consumer(&prod_cons);
  exit(0);
}
