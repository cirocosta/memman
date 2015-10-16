#ifndef MEMMAN__PAGESUBST__SCP_H
#define MEMMAN__PAGESUBST__SCP_H

#include "memman/vpage.h"
#include "memman/common.h"
#include "memman/queue.h"
#include "memman/mmu.h"

#include <time.h>
#include <math.h>
#include <stdlib.h>

static mm_queue_t* scp_queue;

static inline void mm_scp_init(mm_mmu_t* mmu)
{
  scp_queue = mm_queue_create(mmu->pages_count);
}

static inline void mm_scp_destroy(mm_mmu_t* mmu)
{
  mm_queue_destroy(scp_queue);
}

// called when a page must be choosen
static inline mm_vpage_t* mm_scp_algorithm(mm_mmu_t* mmu, uint8_t page)
{
  mm_vpage_t* subst_page;
  unsigned ppage;

  if (mmu->free_pageframes_count) {
    for (unsigned i = 0; i < mmu->pageframes_count; i++) {
      if (mmu->free_pageframes[i]) {
        ppage = mm_mmu_map(mmu, &mmu->pages[page], i);
        break;
      }
    }

    mm_queue_insert(scp_queue, &mmu->pages[page]);
    return &mmu->pages[page];
  }

  while (1) {
    subst_page = (mm_vpage_t*)(mm_queue_front(scp_queue));

    if (!subst_page->r)
      break;

    subst_page->r = 0;
    mm_queue_remove(scp_queue);
    mm_queue_insert(scp_queue, subst_page);
  }

  mm_queue_remove(scp_queue);

  ppage = mm_mmu_map(mmu, &mmu->pages[page], subst_page->phys_page);
  mm_mmu_unmap(mmu, subst_page);
  mm_queue_insert(scp_queue, &mmu->pages[page]);

  return subst_page;
}

static mm_pagesubst_alg_t mm_scp_alg = {
  .init = mm_scp_init,
  .destroy = mm_scp_destroy,
  .algorithm = mm_scp_algorithm,
};

#endif
