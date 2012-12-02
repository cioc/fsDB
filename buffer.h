#ifndef BUFFER_H
#define BUFFER_H

//buffer is a lazily allocated array of objects

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

enum {
	UNALLOCATED,
	USED,
	UNUSED
} BUFFER_ELEM_STATE;

typedef struct {
	BUFFER_ELEM_STATE state;
	void *item;
} buffer_elem;

typedef struct {
	uint32_t max_items;
	size_t item_size;
 	uint32_t item_count;
	buffer_elem *items;
} buffer;

bool init_buffer(buffer *b, size_t );
uint32_t insert(buffer *b, void *data);
void *remove(buffer *b, uint32_t index);

#endif
