#include "memman/segment.h"

mm_segment_t* mm_segment_create(unsigned start, unsigned length)
{
  mm_segment_t* seg = malloc(sizeof(*seg));
  PASSERT(seg, MM_ERR_MALLOC);

  seg->start = start;
  seg->length = length;
  seg->process = NULL;

  return seg;
}

void mm_segment_destroy(mm_segment_t* segment)
{
  if (segment->process)
    mm_process_destroy(segment->process);
  FREE(segment);
}

void mm_segment_show(mm_segment_t* seg)
{
  if (seg)
    fprintf(stderr, "[%u, %u]->", seg->start, seg->length);
  else
    fprintf(stderr, "[ ]->");
}
