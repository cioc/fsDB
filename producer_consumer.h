#ifndef PRODUCER_CONSUMER_H
#define PRODUCER_CONSUMER_H

#include "buffer.h"
#include "pthread.h"

typedef struct {
	buffer *buf;
	pthread_mutex_t *mutex;
	pthread_cond_t *full;
	pthread_cond_t *empty;
} producer_consumer;

#endif
