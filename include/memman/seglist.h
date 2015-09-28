#ifndef MEMMAN__SEGLIST_H
#define MEMMAN__SEGLIST_H

#include "memman/common.h"
#include "memman/dllist.h"
#include "memman/segment.h"

#include "memman/freemem/ff.h"
#include "memman/freemem/nf.h"
#include "memman/freemem/qf.h"

struct mm_seglist_t;
typedef mm_segment_t* (*mm_seglist_func)(mm_dllist_t* freelist, unsigned size);

// if size == 0 ==> all sizes
// TODO in the case of quickfit we might want to have
// more than one list
/* typedef struct mm_hole_list_t { */
/*   mm_dllist_t* list; */
/*   unsigned holes_size; */
/* } mm_hole_list_t; */

typedef struct mm_seglist_t {
  unsigned size;
  mm_dllist_t* processes;
  mm_dllist_t* holes;
  /* mm_hole_list_t holes_lists[4]; */

  mm_algorithms_e algorithm_type;
  mm_seglist_func algorithm;
} mm_seglist_t;

mm_seglist_t* mm_seglist_create(unsigned size, mm_algorithms_e algorithm)
{
  mm_segment_t* hole_seg = mm_segment_create(0, size);
  mm_seglist_t* list = malloc(sizeof(*list));
  PASSERT(list, MM_ERR_MALLOC);

  list->size = size;
  list->processes = mm_dllist_create(NULL);
  list->holes = mm_dllist_create(NULL);

  mm_dllist_insert_after(list->holes, hole_seg);

  list->algorithm_type = algorithm;
  switch (algorithm) {
    case MM_ALG_FREE_FF:
      list->algorithm = &mm_freemem_ff;
      break;
    case MM_ALG_FREE_NF:
    case MM_ALG_FREE_QF:
      LOGERR("Unsupported algorithm");
      exit(EXIT_FAILURE);
      break;
  }

  return list;
}

void mm_seglist_destroy(mm_seglist_t* list)
{
  mm_dllist_destroy(list->holes);
  mm_dllist_destroy(list->processes);
  FREE(list);
}

mm_dllist_t* mm_seglist_search_pos_before(mm_dllist_t* list, mm_segment_t* seg)
{
  return list;
}

void mm_seglist_add_process(mm_seglist_t* list, mm_process_t* process)
{
  mm_segment_t* free_seg_to_alt =
      list->algorithm(list->holes->next, process->b);
  mm_segment_t* process_seg =
      mm_segment_create(free_seg_to_alt->start, process->b);

  mm_dllist_t* process_before =
      mm_seglist_search_pos_before(list->processes, process_seg);
  mm_dllist_insert_after(process_before, process_seg);

  free_seg_to_alt->length -= process->b;
  free_seg_to_alt->start += process->b;
}

#endif
