#ifndef MEMMAN__SEGLIST_H
#define MEMMAN__SEGLIST_H

#include "memman/common.h"
#include "memman/dllist.h"

#include "memman/freemem/ff.h"
#include "memman/freemem/nf.h"
#include "memman/freemem/qf.h"

// TODO rename this. It's more like a "segments_manager"

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

mm_seglist_t* mm_seglist_create(unsigned size, mm_algorithms_e algorithm);
void mm_seglist_destroy(mm_seglist_t* list);
mm_segment_t* mm_seglist_add_process(mm_seglist_t* list, mm_process_t* process);
void mm_seglist_free_process(mm_seglist_t* list, mm_segment_t* process);

#endif
