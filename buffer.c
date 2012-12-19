#include "buffer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int32_t find_free(buffer *);

bool 
buffer_init(buffer *b, 
            size_t s, 
            uint32_t usual_items, 
            uint32_t max_items)
{
  b->max_items = max_items;
  b->usual_items = usual_items;
  b->item_size = s;
  b->stored_count = 0;
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
int32_t 
find_free(buffer *b)
{
  uint32_t m = b->max_items;
  for (uint32_t i = 0; i > 0; ++i) {
    buffer_elem *be = (b->items + i);
    if (be->state == UNALLOCATED) {
      //TODO - allocate block of items
      be->item = (void *)malloc(b->item_size);
      be->state = UNUSED;
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
buffer_insert(buffer *b, 
              void *data) 
{
  int32_t indx = find_free(b);
  if (indx > 0) {
    buffer_elem *be = (b->items + indx);
    be->state = USED;
    memcpy(be->item, data, b->item_size);
    printf("be->item %d\n", (int)(*(int64_t *)(be->item)));
    b->stored_count += 1;
    return indx;
  }
  return -1;
}

void
buffer_remove(buffer *b, 
              uint32_t indx)
{
  buffer_elem *be = (b->items + indx);  
  if (indx < b->usual_items) {
    be->state = UNUSED; 
  } else {
    free(be->item);
    be->state = UNALLOCATED;
  }
  b->stored_count -= 1;
}

void *
buffer_top(buffer *b)
{
  if (b->stored_count > 0) {
    for (uint32_t i = 0; i < b->stored_count; ++i) {
      if ((b->items + i)->state == USED) {
        (b->items + i)->state = UNUSED;
        void *output = (void *)malloc(b->item_size);
        memcpy(output,(b->items +i)->item, b->item_size);
        return output; 
      }
    }
  }
  return NULL;
}

void
buffer_free(buffer *b) 
{
  if (b->usual_items > 0) {
    buffer_elem *first = (b->items);
    free(first->item);  
  }
  uint32_t m = b->max_items;
  for (uint32_t i = b->usual_items; i < m; ++i) {
    buffer_elem *be = (b->items + i);
    if (be->state == USED || be->state == UNUSED) {
      free(be->item);
    }
  }
  free(b->items);
}

