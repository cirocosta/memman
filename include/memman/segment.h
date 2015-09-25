
#ifndef MEMMAN__SEGMENT_H
#define MEMMAN__SEGMENT_H 

/**
 * Entry in the llist that represents our memory.
 * It might either be an empty memory seg or
 * a non-empty one, owned by a process.
 */

#include "memman/common.h"

typedef struct mm_seg_t {
  mm_seg_type type;
  unsigned start;
  unsigned length;

  struct mm_seg_t* next;
} mm_seg_t;

#endif
