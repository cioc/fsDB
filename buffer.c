#include "buffer.h"
#include <string.h>

static buffer_elem *find_free(buffer *b);

bool 
init_buffer(buffer *b, size_t s, uint32_t max_items)
{
  b->max_items = max_items;
	b->item_size = s;
	b->item_count = 0;
  b->items = (buffer_elem *)malloc(sizeof(buffer_elem) * max_items);
	if (b->items != NULL) {
		for (uint32_t i = 0; i < max_items; ++i) {
			buffer_elem *b = (b->items + i); 	
			b->state = UNALLOCATED;	
		}
		return true;
	} else {
		return false;
	}
}

buffer_elem *
find_free(buffer *b)
{
	uint32_t m = b->max_items;
	if (b->item_count < m) {
		for (uint32_t i = 0; i < m; ++i) {
			buffer_elem *b = (b->items + i);
			if (b->state == UNALLOCATED) {
			  //TODO - START HERE, SO hungry
				b->item = (void *)malloc(sizeof(b->item_size) * 	
			}
			if (b->state == UNUSED) {
			
			}
		}
	}
	return NULL;
}

uint32_t 
insert(buffer *b, void *data) 
{
  void * find_free(b)	
}

void *
remove(buffer *b, uint32_t index)
{

}


