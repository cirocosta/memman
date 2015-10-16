#ifndef MEMMAN__PAGESUBST_NRUP_H
#define MEMMAN__PAGESUBST_NRUP_H

#include "memman/vpage.h"
#include "memman/common.h"
#include "memman/mmu.h"

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

static inline mm_vpage_t* mm_nrup_algorithm(mm_mmu_t* mmu, uint8_t page)
{
  unsigned i = 0;
  unsigned pos;
  unsigned ppage;
  mm_vpage_t* subst_page;

  if (mmu->free_pageframes_count) {
    for (unsigned i = 0; i < mmu->pageframes_count; i++){ 
      if (mmu->free_pageframes[i]) {
        ppage = mm_mmu_map(mmu, &mmu->pages[page], i);
        break;
      }
    }

    return &mmu->pages[page];
  }

  nrup_lists.class0_counter = 0;
  nrup_lists.class1_counter = 0;

  for (; i < mmu->pages_count; i++) {
    if (!mmu->pages[i].p)
      continue;

    if (!mmu->pages[i].r)
      nrup_lists.class0[nrup_lists.class0_counter++] = &mmu->pages[i];
    else
      nrup_lists.class1[nrup_lists.class1_counter++] = &mmu->pages[i];
  }

  if (nrup_lists.class0_counter) {
    pos = rand() % nrup_lists.class0_counter;
    subst_page = nrup_lists.class0[pos];
  } else {
    pos = rand() % nrup_lists.class1_counter;
    subst_page = nrup_lists.class1[pos];
  }

  ppage = mm_mmu_map(mmu, &mmu->pages[page], subst_page->phys_page);
  mm_mmu_unmap(mmu, subst_page);

  return subst_page;
}

#endif
