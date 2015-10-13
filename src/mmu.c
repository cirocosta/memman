#include "memman/mmu.h"

mm_mmu_t* mm_mmu_create(unsigned vsize, unsigned psize,
                        unsigned page_frame_size,
                        mm_mmu_pr_alg replacement_alg)
{
  unsigned i = 0;

  mm_mmu_t* mmu = malloc(sizeof(*mmu));
  PASSERT(mmu, MM_ERR_MALLOC);

  mmu->replacement_alg = replacement_alg;
  mmu->offset_size_bits = log2(page_frame_size);
  mmu->page_size_bits = log2(vsize) - mmu->offset_size_bits;
  mmu->frame_size_bits = log2(psize) - mmu->offset_size_bits;
  mmu->pages_count = vsize / page_frame_size;
  mmu->offset_mask = (1 << mmu->offset_size_bits) - 1;

  mmu->pages = calloc(mmu->pages_count, sizeof(*mmu->pages));
  PASSERT(mmu->pages, MM_ERR_MALLOC);

  for (; i < mmu->pages_count; i++) {
    mmu->pages[i].r = 0;
    mmu->pages[i].m = 0;
    mmu->pages[i].p = 0;
    mmu->pages[i].phys_page = UINT_MAX;
  }

  return mmu;
}

void mm_mmu_destroy(mm_mmu_t* mmu)
{
  FREE(mmu->pages);
  FREE(mmu);
}

void mm_mmu_map(mm_mmu_t* mmu, mm_vpage_t* vpage, unsigned phys_page)
{
  vpage->p = 1;
  vpage->r = 0;
  vpage->m = 0;
  vpage->phys_page = phys_page;
}

unsigned mm_mmu_access(mm_mmu_t* mmu, unsigned position)
{
  uint8_t page = position >> mmu->offset_size_bits;
  uint8_t offset = position & mmu->offset_mask;

  if (mmu->pages[page].p)
    return mmu->pages[page].phys_page + offset;

  mm_vpage_t* subst_page = mmu->replacement_alg(mmu->pages, mmu->pages_count);
  mm_mmu_map(mmu, &mmu->pages[page], subst_page->phys_page);
  mm_mmu_unmap(mmu, subst_page);

  return mm_mmu_access(mmu, position);
}
