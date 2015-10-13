#include "memman/common.h"
#include "memman/mmu.h"

void test1()
{
  unsigned i = 0;
  mm_mmu_t* mmu = mm_mmu_create(64, 16);

  ASSERT(mmu->pages, "");
  ASSERT(mmu->pages_count == 4, "");

  for (; i < mmu->pages_count; i++) {
    ASSERT(mmu->pages[0].m == 0, "");
    ASSERT(mmu->pages[0].r == 0, "");
    ASSERT(mmu->pages[0].p == 0, "");
  }

  ASSERT(mmu->offset_size_bits == 2, "");
  ASSERT(mmu->page_size_bits == 4, "");
  ASSERT(mmu->offset_size_bits + mmu->page_size_bits == log2(64), "");

  mm_mmu_destroy(mmu);
}

void test2()
{
  mm_mmu_t* mmu = mm_mmu_create(64, 16);

  ASSERT(mmu->pages[0].phys_pos == UINT_MAX, "");
  ASSERT(mmu->pages[0].p == 0, "");

  mm_mmu_map(mmu, 0, 3);

  ASSERT(mmu->pages[0].p == 1, "");
  ASSERT(mmu->pages[0].m == 0, "");
  ASSERT(mmu->pages[0].r == 0, "");
  ASSERT(mmu->pages[0].phys_pos == 3, "");

  mm_mmu_unmap(mmu, 0);

  ASSERT(mmu->pages[0].p == 0, "");
  ASSERT(mmu->pages[0].m == 0, "");
  ASSERT(mmu->pages[0].r == 0, "");

  mm_mmu_destroy(mmu);
}

int main()
{
  TEST(test1, "Initialization");
  TEST(test2, "Mapping");

  return 0;
}
