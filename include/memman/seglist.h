#ifndef MEMMAN__SEGLIST_H
#define MEMMAN__SEGLIST_H

#include "memman/common.h"
#include "memman/dllist.h"

#include "memman/freemem/ff.h"
#include "memman/freemem/nf.h"
#include "memman/freemem/qf.h"

struct mm_seglist_t;
typedef mm_segment_t* (*mm_seglist_func)(mm_dllist_t* freelist, unsigned size);

typedef struct mm_seglist_t {
  unsigned size;
  mm_dllist_t* processes;
  mm_dllist_t* holes;

  mm_algorithms_e algorithm_type;
  mm_seglist_func algorithm;
} mm_seglist_t;

mm_seglist_t* mm_seglist_create(unsigned size, mm_algorithms_e algorithm);
void mm_seglist_destroy(mm_seglist_t* list);
mm_segment_t* mm_seglist_add_process(mm_seglist_t* list, mm_process_t* process);
void mm_seglist_free_process(mm_seglist_t* list, mm_segment_t* segment);
mm_segment_t* mm_seglist_search_process(mm_seglist_t* list,
                                        mm_process_t* process);

inline static mm_segment_t* mm_seglist_add_proc16(mm_seglist_t* list,
                                                  mm_process_t* proc)
{
  proc->b = (((proc->b - 1) / 16 | 0) + 1) * 16;
  return mm_seglist_add_process(list, proc);
}

#endif
