#include "memman/mmu.h"

mm_mmu_t* mm_mmu_create(unsigned vsize, unsigned psize,
                        unsigned page_frame_size, mm_mmu_pr_alg replacement_alg)
{
  unsigned i;

  mm_mmu_t* mmu = malloc(sizeof(*mmu));
  PASSERT(mmu, MM_ERR_MALLOC);

  mmu->pages_count = vsize / page_frame_size;
  mmu->pageframes_count = psize / page_frame_size;
  mmu->pages = calloc(mmu->pages_count, sizeof(*mmu->pages));
  PASSERT(mmu->pages, MM_ERR_MALLOC);

  mmu->replacement_alg = replacement_alg;
  mmu->offset_size_bits = log2(page_frame_size);
  mmu->page_size_bits = log2(vsize) - mmu->offset_size_bits;
  mmu->frame_size_bits = log2(psize) - mmu->offset_size_bits;
  mmu->offset_mask = (1 << mmu->offset_size_bits) - 1;

  mmu->free_pageframes =
      calloc(mmu->pageframes_count, sizeof(*mmu->free_pageframes));
  PASSERT(mmu->free_pageframes, MM_ERR_MALLOC);
  mmu->free_pageframes_count = mmu->pageframes_count;

  for (i = 0; i < mmu->pages_count; i++)
    mmu->pages[i] = mm_vpage_zeroed;
  for (i = 0; i < mmu->pageframes_count; i++)
    mmu->free_pageframes[i] = 1;

  return mmu;
}

void mm_mmu_set_replacememt_alg(mm_mmu_t* mmu, mm_mmu_pr_alg replacement_alg)
{
  mmu->replacement_alg = replacement_alg;
}

void mm_mmu_destroy(mm_mmu_t* mmu)
{
  FREE(mmu->pages);
  FREE(mmu->free_pageframes);
  FREE(mmu);
}

unsigned mm_mmu_map(mm_mmu_t* mmu, mm_vpage_t* vpage, unsigned phys_page)
{
  vpage->p = 1;
  vpage->r = vpage->m = 0;
  vpage->phys_page = phys_page;

  mmu->free_pageframes[phys_page] = 0;
  mmu->free_pageframes_count--;

  return phys_page;
}

void mm_mmu_unmap(mm_mmu_t* mmu, mm_vpage_t* vpage)
{
  vpage->p = 0;

  mmu->free_pageframes[vpage->phys_page] = 1;
  mmu->free_pageframes_count++;
}

unsigned mm_mmu_map_free_pageframe(mm_mmu_t* mmu, mm_vpage_t* page)
{
  for (unsigned i = 0; i < mmu->pageframes_count; i++)
    if (mmu->free_pageframes[i])
      return mm_mmu_map(mmu, page, i);

  ASSERT(0, "should have found a valid free_pageframe");
}

unsigned mm_mmu_access(mm_mmu_t* mmu, unsigned position, unsigned* mb)
{
  uint8_t page = position >> mmu->offset_size_bits;
  uint8_t offset = position & mmu->offset_mask;
  unsigned ppage;

  if (mmu->pages[page].p)
    return (mmu->pages[page].phys_page << mmu->offset_size_bits) + offset;

  if (mmu->free_pageframes_count) {
    ppage = mm_mmu_map_free_pageframe(mmu, &mmu->pages[page]);
    if (mb)
      *mb = ppage;

    return mm_mmu_access(mmu, position, mb);
  }

  mm_vpage_t* subst_page = mmu->replacement_alg(mmu->pages, mmu->pages_count);

  ppage = mm_mmu_map(mmu, &mmu->pages[page], subst_page->phys_page);
  mm_mmu_unmap(mmu, subst_page);

  if (mb)
    *mb = ppage;

  return mm_mmu_access(mmu, position, mb);
}
