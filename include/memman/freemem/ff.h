#ifndef MEMMAN__FREEMEM__FF_H
#define MEMMAN__FREEMEM__FF_H

#include "memman/common.h"
#include "memman/segment.h"
#include "memman/dllist.h"

mm_segment_t* mm_freemem_ff(mm_dllist_t* freelist, unsigned size)
{
  mm_dllist_t* list = freelist;
  mm_segment_t* seg;

  do {
    seg = (mm_segment_t*)list->data;
    if (seg->length >= size)
      return seg;
  } while ((list = list->next) && list->data);

  return NULL; // no space found.
}

#endif
