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

bool buffer_init(buffer *, size_t, uint32_t, uint32_t);
uint32_t buffer_insert(buffer *, void *);
void buffer_remove(buffer *, uint32_t);
void *buffer_top(buffer *);
void buffer_free(buffer *);
#endif
