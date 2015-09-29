#ifndef MEMMAN__FREEMEM__NF_H
#define MEMMAN__FREEMEM__NF_H 

#include "memman/common.h"
#include "memman/segment.h"
#include "memman/dllist.h"
#include "memman/freemem/ff.h"

// TODO
// -  problems: receiving the state
// -  problems: compatible api

/* mm_segment_t* mm_freemem_nf(mm_dllist_t* freelist, unsigned size) */
/* { */
/*   mm_segment_t* seg = NULL; */

  // breaks the list in two parts.
  // calls FF after the last found place
  // if no place found
  // calls FF in the first part of the list
  //
  // start----------last_found--------
  //        becomes
  // last_found----------
  // start----------

  /* return seg; */
/* } */

#endif
