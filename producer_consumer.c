#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include "producer_consumer.h"
#include "buffer.h"
#include "thread_pool.h"

#define NORMAL_BUFFER_ELEMS 100
#define MAX_BUFFER_ELEMS 1000

static void *internal_consume(void *);

void 
produce(producer_consumer *prod_cons, void *obj)
{ 
  printf("ENTER PRODUCE\n");
  pthread_mutex_lock(prod_cons->mutex);
  buffer_insert(prod_cons->buf, obj); 
  pthread_mutex_unlock(prod_cons->mutex);
  printf("EXIT PRODUCE\n");
}

void *
internal_consume(void *prod_cons)
{
  producer_consumer * pc = (producer_consumer *)prod_cons;
  while(1) {
    pthread_mutex_lock(pc->mutex);
    void *obj = buffer_top(pc->buf);
    pthread_mutex_unlock(pc->mutex);
    pc->consume_function(obj);
  }
}


bool 
init_producer_consumer( producer_consumer *prod_cons,
                        uint32_t producers,
                        uint32_t consumers,
                        size_t size_of_obj,
                        void *(*produce_function)(void *),
                        void *(*consume_function)(void *))
{
  prod_cons->produce_function = produce_function;
  prod_cons->consume_function = consume_function;
  prod_cons->producer_pool = (thread_pool *)malloc(sizeof(thread_pool));
  prod_cons->consumer_pool = (thread_pool *)malloc(sizeof(thread_pool));
  if (prod_cons->producer_pool == NULL || prod_cons->consumer_pool == NULL) {
  return false;
  }
  if (!init_pool( prod_cons->producer_pool,
                  producers,
                  produce_function,
                  NULL) || 
      !init_pool( prod_cons->consumer_pool,
                  consumers,
                  internal_consume,
                  prod_cons)) {
    printf("POOL(S) FAILED TO INIT\n");
    return false;
  }
  prod_cons->buf = (buffer *)malloc(sizeof(buffer)); 
  if (prod_cons->buf != NULL && !buffer_init( prod_cons->buf, 
                                              size_of_obj, 
                                              NORMAL_BUFFER_ELEMS,
                                              MAX_BUFFER_ELEMS)) {
    printf("BUFFER INIT FAILURE\n");
    return false;
  }
  prod_cons->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)); 
  if (pthread_mutex_init(prod_cons->mutex, NULL) == 0) {
    return true;
  }
  return false;
} //END init_producer_consumer

void 
start_producer_consumer(producer_consumer *prod_cons)
{
  printf("TACO TACO TACO\n");
  start_pool(prod_cons->producer_pool);
  start_pool(prod_cons->consumer_pool);           
  wait_pool(prod_cons->producer_pool);
  wait_pool(prod_cons->consumer_pool);
}
