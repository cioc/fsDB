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
  while (1) {
    printf("LISTENER\n");
    int64_t number = 5;
    produce(&prod_cons,(void *) number);
  }
}

void *
echo_handler(void *data)
{
  printf("Woopie number: %d\n", (int)(int64_t)(data));
}

int 
main(int argc, char **args)
{
  printf("STARTING ECHO SERVER\n");
  bool init_result = init_producer_consumer(&prod_cons, 
                                            1, 
                                            5, 
                                            sizeof(int32_t),
                                            echo_listener,
                                            echo_handler);
  if (init_result) {  
    printf("INIT SUCCESS\n");
    start_producer_consumer(&prod_cons);
    exit(0);
  } else {
    printf("INIT FAILURE\n");
    exit(1);
  }
}
