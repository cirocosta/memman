#ifndef MEMMAN__SEGLIST_H
#define MEMMAN__SEGLIST_H 

#include "memman/common.h"
#include "memman/dllist.h"
#include "memman/segment.h"

struct mm_seglist_t;
typedef void (*mm_seglist_func)(struct mm_seglist_t* seglist, mm_segment_t*);

// if size == 0 ==> all sizes 
typedef struct mm_hole_list_t {
  mm_dllist_t* list;
  unsigned hole_size;
} mm_hole_list_t; 

typedef struct mm_seglist_t {
  // TODO in the case of quickfit we might want to have
  // more than one list
  mm_dllist_t* processes;
  mm_hole_list_t holes_lists[4];

  mm_seglist_func alloc;
  mm_seglist_func release;
} mm_seglist_t;


mm_seglist_t* mm_seglist_create();
mm_seglist_t* mm_seglist_destroy();


#endif
