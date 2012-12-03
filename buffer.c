#include "buffer.h"
#include <string.h>

static uint32_t find_free(buffer *);

bool 
init_buffer(buffer *b, 
						size_t s, 
						uint32_t usual_items, 
						uint32_t max_items)
{
  b->max_items = max_items;
	b->usual_items = usual_items;
	b->item_size = s;
  b->items = (buffer_elem *)malloc(sizeof(buffer_elem) * max_items);
	if (b->items != NULL) {
		for (uint32_t i = 0; i < max_items; ++i) {
			buffer_elem *be = (b->items + i); 	
			if (i < usual_items) {
				be->state = UNUSED;
			} else {
				be->state = UNALLOCATED;	
			}
		}
		uint8_t *preallocate = (uint8_t *)malloc(s * usual_items);
		if (preallocate == NULL) {
			free(b->items);
			return false;
		}
		for (uint32_t i = 0; i < usual_items; ++i) {
			buffer_elem *be = (b->items + i);
			be->item = (void *)(preallocate + (s * i));
		}
		return true;
	} else {
		return false;
	}
}

//TODO - improve on search
uint32_t 
find_free(buffer *b)
{
	uint32_t m = b->max_items;
	for (uint32_t i = 0; i < m; ++i) {
		buffer_elem *be = (b->items + i);
		if (be->state == UNALLOCATED) {
			//TODO - allocate block of items
			be->item = (void *)malloc(b->item_size);
			if (be->item != NULL) {
				be->state = UNUSED;
				return i;
			}
			return -1;
		}
		if (be->state == UNUSED) {
			return i;			
		}
	}
	return -1;
}

uint32_t 
insert_buffer(buffer *b, 
							void *data) 
{
  uint32_t indx = find_free(b);
	if (indx > 0) {
		buffer_elem *be = (b->items + indx);
		memcpy(be->item, data, b->item_size);
		return indx;
	}
	return -1;
}

void
remove_buffer(buffer *b, 
							uint32_t indx)
{
  buffer_elem *be = (b->items + indx);	
  if (indx < b->usual_items) {
		be->state = UNUSED;	
	} else {
		free(be->item);
		be->state = UNALLOCATED;
	}
}

void
free_buffer(buffer *b) 
{
	if (b->usual_items > 0) {
		buffer_elem *first = (b->items);
		free(first->item);	
	}
	uint32_t m = b->max_items;
	for (uint32_t i = b->usual_items; i <	m; ++i) {
	  buffer_elem *be = (b->items + i);
	  if (be->state == USED || be->state == UNUSED) {
			free(be->item);
		}
	}
	free(b->items);
}

