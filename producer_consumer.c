#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <pthread.h>
#include "producer_consumer.h"
#include "buffer.h"
#include "thread_pool.h"

#define NORMAL_BUFFER_ELEMS 100
#define MAX_BUFFER_ELEMS 1000

static void *internal_consume(void *);

void 
produce(producer_consumer *prod_cons, void *obj)
{
  pthread_cond_wait(prod_cons->empty, prod_cons->mutex_empty);  
  pthread_mutex_lock(prod_cons->mutex);
  buffer_insert(prod_cons->buf, obj); 
  pthread_mutex_unlock(prod_cons->mutex);
  pthread_cond_signal(prod_cons->full);
}

void *
internal_consume(void *prod_cons)
{
  producer_consumer * pc = (producer_consumer *)prod_cons;
  while(1) {
    pthread_cond_wait(pc->full, pc->mutex_full);
    pthread_mutex_lock(pc->mutex);
    void *obj = buffer_top(pc->buf);
    pthread_mutex_unlock(pc->mutex);
    pc->consume_function(obj);
    pthread_cond_signal(pc->empty);
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
  prod_cons->mutex_full = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  prod_cons->mutex_empty = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  prod_cons->full = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
  prod_cons->empty = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
  
  int32_t r1 = pthread_mutex_init(prod_cons->mutex, NULL);
  int32_t r2 = pthread_mutex_init(prod_cons->mutex_full, NULL);
  int32_t r3 = pthread_mutex_init(prod_cons->mutex_empty, NULL);
  int32_t r4 = pthread_cond_init(prod_cons->full, NULL);
  int32_t r5 = pthread_cond_init(prod_cons->empty, NULL);
  if ((r1==0) && (r2==0) && (r3==0) && (r4==0) && (r5==0)) {
    return true;
  }
  printf("MUTEX / COND VARIABLE SETUP FAILURE\n");
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
