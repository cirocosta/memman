#include "memman/mmu.h"

mm_mmu_t* mm_mmu_create(unsigned vsize, unsigned page_frame_size)
{
  unsigned i = 0;

  mm_mmu_t* mmu = malloc(sizeof(*mmu));
  PASSERT(mmu, MM_ERR_MALLOC);

  mmu->page_size_bits = log2(page_frame_size);
  mmu->offset_size_bits = log2(vsize) - mmu->page_size_bits;
  mmu->pages_count = vsize / page_frame_size;

  mmu->pages = calloc(mmu->pages_count, sizeof(*mmu->pages));
  PASSERT(mmu->pages, MM_ERR_MALLOC);

  for (; i < mmu->pages_count; i++) {
    mmu->pages[i].r = 0;
    mmu->pages[i].m = 0;
    mmu->pages[i].p = 0;
    mmu->pages[i].phys_pos = UINT_MAX;
  }

  return mmu;
}

void mm_mmu_destroy(mm_mmu_t* mmu)
{
  FREE(mmu->pages);
  FREE(mmu);
}

void mm_mmu_map(mm_mmu_t* mmu, unsigned vpage, unsigned phys_page)
{
  mmu->pages[vpage].p = 1;
  mmu->pages[vpage].phys_pos = phys_page;
}

void mm_mmu_unmap(mm_mmu_t* mmu, unsigned vpage)
{
  mmu->pages[vpage].p = 0;
}

/* unsigned mm_mmu_access(mm_mmu_t* mmu, unsigned position) */
/* { */
/*   return 0; */
/* } */

