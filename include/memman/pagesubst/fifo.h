#ifndef MEMMAN__PAGESUBST__FIFO_H
#define MEMMAN__PAGESUBST__FIFO_H

#include "memman/vpage.h"
#include "memman/common.h"
#include "memman/queue.h"
#include "memman/mmu.h"

#include <time.h>
#include <math.h>
#include <stdlib.h>


static mm_queue_t* fifo_queue;

static inline void mm_fifo_init(unsigned pages_count) {
  fifo_queue = mm_queue_create(pages_count);
}

static inline void mm_fifo_destroy(unsigned pages_count) {
  mm_queue_destroy(fifo_queue);
}

// called when a page must be choosen
static inline mm_vpage_t* mm_fifo_algorithm(mm_mmu_t* mmu, uint8_t page)
{
  unsigned ppage;

  if (mmu->free_pageframes_count) {
    for (unsigned i = 0; i < mmu->pageframes_count; i++)
      if (mmu->free_pageframes[i])
        ppage = mm_mmu_map(mmu, &mmu->pages[page], i);

    mm_queue_insert(fifo_queue, &mmu->pages[page]);
    return &mmu->pages[page];
  }

  mm_vpage_t* subst_page = (mm_vpage_t*)(mm_queue_front(fifo_queue));
  mm_queue_remove(fifo_queue);

  ppage = mm_mmu_map(mmu, &mmu->pages[page], subst_page->phys_page);
  mm_mmu_unmap(mmu, subst_page);
  mm_queue_insert(fifo_queue,  &mmu->pages[page]);
 
  return subst_page;
}

#endif
