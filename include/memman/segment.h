
#ifndef MEMMAN__SEGMENT_H
#define MEMMAN__SEGMENT_H

#include "memman/common.h"
#include "memman/process.h"

/**
 * INVARIANTS:
 *  - segment represents a process ==> seg->process != NULL
 *  - segment represets a hole ==> seg->process == NULL
 */

typedef struct mm_segment_t {
  unsigned start;
  unsigned length;
  mm_process_t* process;
} mm_segment_t;

mm_segment_t* mm_segment_create(unsigned start, unsigned length);
void mm_segment_destroy(mm_segment_t* segment);

#endif
