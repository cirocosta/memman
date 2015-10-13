#include "memman/common.h"
#include "memman/mmu.h"

/**
 * -----------------------
 * |       |             |   6b    - virtual addr
 * -----------------------
 *  (2b p)   (4b offset)
 *
 *    --------------------
 *    |    |             |   5b    - physical addr
 *    --------------------
 *    (1b)    (4b offset)
 */
void test1()
{
  unsigned i = 0;
  unsigned virtual = 64;
  unsigned physical = 32;
  unsigned page_frame_size = 16;
  mm_mmu_t* mmu = mm_mmu_create(virtual, physical, page_frame_size, NULL);

  ASSERT(mmu->pages, "");
  ASSERT(mmu->pages_count == 4, "");

  for (; i < mmu->pages_count; i++) {
    ASSERT(mmu->pages[0].m == 0, "");
    ASSERT(mmu->pages[0].r == 0, "");
    ASSERT(mmu->pages[0].p == 0, "");
  }

  ASSERT(mmu->offset_size_bits == 4, "");
  ASSERT(mmu->page_size_bits == 2, "");
  ASSERT(mmu->frame_size_bits == 1, "");
  ASSERT(mmu->offset_size_bits + mmu->page_size_bits == log2(virtual), "");
  ASSERT(mmu->offset_size_bits + mmu->frame_size_bits == log2(physical), "");

  mm_mmu_destroy(mmu);
}

void test2()
{
  mm_mmu_t* mmu = mm_mmu_create(64, 32, 16, NULL);

  ASSERT(mmu->pages[0].phys_page == UINT_MAX, "");
  ASSERT(mmu->pages[0].p == 0, "");

  mm_mmu_map(mmu, &mmu->pages[0], 3);

  ASSERT(mmu->pages[0].p == 1, "");
  ASSERT(mmu->pages[0].m == 0, "");
  ASSERT(mmu->pages[0].r == 0, "");
  ASSERT(mmu->pages[0].phys_page == 3, "");

  mm_mmu_unmap(mmu, &mmu->pages[0]);

  ASSERT(mmu->pages[0].p == 0, "");
  ASSERT(mmu->pages[0].m == 0, "");
  ASSERT(mmu->pages[0].r == 0, "");

  mm_mmu_destroy(mmu);
}

//  (virtual)        (physical)
// 3  : 48-65
// 2  : 32-47
// 1  : 16-31 ---   1 : 16-31
// 0  :  0-15   |-- 0 :  0-15
void test3()
{
  mm_mmu_t* mmu = mm_mmu_create(64, 32, 16, NULL);

  mm_mmu_map(mmu, &mmu->pages[1], 0);

  ASSERT(mm_mmu_access(mmu, 16) == 0, "");
  ASSERT(mm_mmu_access(mmu, 17) == 1, "");
  ASSERT(mm_mmu_access(mmu, 18) == 2, "");
  ASSERT(mm_mmu_access(mmu, 31) == 15, "");

  mm_mmu_unmap(mmu, &mmu->pages[1]);
  mm_mmu_map(mmu, &mmu->pages[2], 0);

  ASSERT(mm_mmu_access(mmu, 32) == 0, "");
  ASSERT(mm_mmu_access(mmu, 33) == 1, "");
  ASSERT(mm_mmu_access(mmu, 47) == 15, "");

  mm_mmu_destroy(mmu);
}

int main()
{
  TEST(test1, "Initialization");
  TEST(test2, "Mapping and unmapping");
  TEST(test3, "Access to mapped area");
  // TODO
  /* TEST(test4, "Access to unmapped area w/ phys space"); */
  /* TEST(test5, "Access to unmapped area w/out phys space"); */

  return 0;
}
