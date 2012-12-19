#ifndef PRODUCER_CONSUMER_H
#define PRODUCER_CONSUMER_H

#include "buffer.h"
#include "thread_pool.h"
#include <pthread.h>
#include <semaphore.h>

typedef struct {
  buffer *buf;
  pthread_mutex_t *mutex;
  thread_pool *producer_pool;
  thread_pool *consumer_pool;
  void *(*produce_function)(void *);
  void *(*consume_function)(void *);
} producer_consumer;

void produce(producer_consumer *, void *);
bool init_producer_consumer(producer_consumer *prod_cons,
                        uint32_t producers,
                        uint32_t consumers,
                        size_t size_of_obj,
                        void *(*produce_function)(void *),
                        void *(*consume_function)(void *));
void start_producer_consumer(producer_consumer *);
#endif
