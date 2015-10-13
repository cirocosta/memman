#ifndef MEMMAN__PAGESUBST_NRUP_H
#define MEMMAN__PAGESUBST_NRUP_H

#include "memman/vpage.h"
#include "memman/common.h"

#include <time.h>
#include <math.h>
#include <stdlib.h>

typedef struct mm_nrup_lists_t {
  mm_vpage_t** class0;
  mm_vpage_t** class1;

  unsigned class0_counter;
  unsigned class1_counter;
} mm_nrup_lists_t;

static mm_nrup_lists_t nrup_lists = { 0 };

static inline void mm_nrup_init(unsigned pages_count)
{
  srand(time(NULL));
  nrup_lists.class0 = calloc(pages_count, sizeof(*nrup_lists.class0));
  nrup_lists.class1 = calloc(pages_count, sizeof(*nrup_lists.class1));

  PASSERT(nrup_lists.class0, MM_ERR_MALLOC);
  PASSERT(nrup_lists.class1, MM_ERR_MALLOC);
}

static inline void mm_nrup_destroy(unsigned pages_count)
{
  FREE(nrup_lists.class0);
  FREE(nrup_lists.class1);
}

static inline mm_vpage_t* mm_nrup_algorithm(mm_vpage_t* pages, unsigned pages_count)
{
  unsigned i = 0;
  unsigned pos;

  nrup_lists.class0_counter = 0;
  nrup_lists.class1_counter = 0;

  for (; i < pages_count; i++) {
    if (!pages[i].p)
      continue;

    if (!pages[i].r)
      nrup_lists.class0[nrup_lists.class0_counter++] = &pages[i];
    else
      nrup_lists.class1[nrup_lists.class1_counter++] = &pages[i];
  }

  if (nrup_lists.class0_counter) {
    pos = rand() % nrup_lists.class0_counter;
    return nrup_lists.class0[pos];
  }

  pos = rand() % nrup_lists.class1_counter;
  return nrup_lists.class1[pos];
}

#endif
