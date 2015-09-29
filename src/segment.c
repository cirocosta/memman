#include "memman/segment.h"

mm_segment_t* mm_segment_create(unsigned start, unsigned length)
{
  mm_segment_t* seg = malloc(sizeof(*seg));
  PASSERT(seg, MM_ERR_MALLOC);

  seg->start = start;
  seg->length = length;

  return seg;
}

void mm_segment_destroy(mm_segment_t* segment) { FREE(segment); }
