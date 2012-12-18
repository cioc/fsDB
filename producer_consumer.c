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
	insert_buffer(prod_cons->buf, obj);	
	pthread_mutex_unlock(prod_cons->mutex);
	pthread_cond_signal(prod_cons->full);
}

//TODO - ADD ENDLESS LOOP
void *
internal_consume(void *prod_cons)
{
	producer_consumer * pc = (producer_consumer *)prod_cons;
	pthread_cond_wait(pcop->prod_cons->full, pcop->prod_cons->mutex_full);
	pthread_mutex_lock(pcop->prod_cons->mutex);
	void *obj = //TODO - IMPLEMENT A REMOVE_FIRST OPERATION
	prod_cons->consume_function(obj);
	pthread_mutex_unlock(pcop->prod_cons->mutex);
	pthread_cond_signal(pcop->prod_cons->empty);
}


bool 
init_producer_consumer(	producer_consumer *prod_cons,
												uint32_t producers,
												uint32_t consumers,
												size_t size_of_obj,
												void *(*)(void *) produce_function,
												void *(*)(void *) consume_function)
{
	prod_cons->produce_function = produce_function;
	prod_cons->consume_function = consume_function;
	prod_cons->producer_pool = (thread_pool *)malloc(sizeof(thread_pool));
	prod_cons->consumer_pool = (thread_pool *)malloc(sizeof(thread_pool));
	if (prod_cons->producer_pool == NULL || prod_cons->consumer_pool == NULL)
	{
		return false;
	}
	if (!init_pool(prod_cons->producer_pool,
								producers,
								produce_function,
								NULL) || 
			!init_pool(prod_cons->consumer_pool,
								 consumers,
								 internal_consume,
								 prod_cons)
			) {
		return false;
	}
	
	if (!init_buffer(prod_cons->buffer, 
									 size_of_obj, 
									 NORMAL_BUFFER_ELEMS,
									 MAX_BUFFER_ELEMS)
			) {
		return false;
	}

	
	int32_t r1 = pthread_mutex_init(prod_cons->mutex, NULL);
 	int32_t r2 = pthread_cond_init(prod_cons->full, NULL);
	int32_t r3 = pthread_cond_init(prod_cons->empty, NULL);
	int32_t r4 = pthread_mutex_init(prod_cons->mutex_full, NULL);
	int32_t r5 = pthread_mutex_init(prod_cons->mutex_empty, NULL);
	if (r1 && r2 && r3 && r4 && r5) {
		return true;
	}
	return false;
} //END init_producer_consumer

void 
start_producer_consumer(producer_consumer *prod_cons)
{
	start_pool(prod_cons->producer_pool);
	start_pool(prod_cons->consumer_pool);						
}
