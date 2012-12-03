#ifndef BUFFER_H
#define BUFFER_H

//buffer is a semi-lazily allocated array of structs

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
	UNALLOCATED,
	USED,
	UNUSED
} buffer_enum_state;

typedef struct {
	buffer_enum_state state;
	void *item;
} buffer_elem;

typedef struct {
	uint32_t max_items;
	uint32_t usual_items;
	size_t item_size;
	buffer_elem *items;
} buffer;

bool init_buffer(buffer *, size_t, uint32_t, uint32_t);
uint32_t insert_buffer(buffer *, void *);
void remove_buffer(buffer *, uint32_t);
void free_buffer(buffer *);
#endif
