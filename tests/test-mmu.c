#include "memman/common.h"
#include "memman/mmu.h"
#include "memman/pagesubst/nrup.h"
#include "memman/pagesubst/fifo.h"

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
  ASSERT(mmu->pageframes_count == 2, "");
  ASSERT(mmu->free_pageframes_count == 2, "");
  ASSERT(mmu->free_pageframes[0] == 1, "");
  ASSERT(mmu->free_pageframes[1] == 1, "");

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

  ASSERT(mmu->free_pageframes_count == mmu->pageframes_count, "");
  ASSERT(mmu->pages[0].phys_page == 0, "");
  ASSERT(mmu->pages[0].p == 0, "");

  mm_mmu_map(mmu, &mmu->pages[0], 3);

  ASSERT(mmu->free_pageframes_count == mmu->pageframes_count - 1, "");
  ASSERT(mmu->pages[0].p == 1, "");
  ASSERT(mmu->pages[0].m == 0, "");
  ASSERT(mmu->pages[0].r == 0, "");
  ASSERT(mmu->pages[0].phys_page == 3, "");

  mm_mmu_unmap(mmu, &mmu->pages[0]);

  ASSERT(mmu->free_pageframes_count == mmu->pageframes_count, "");
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

  ASSERT(mm_mmu_access(mmu, 16, NULL) == 0, "");
  ASSERT(mm_mmu_access(mmu, 17, NULL) == 1, "");
  ASSERT(mm_mmu_access(mmu, 18, NULL) == 2, "");
  ASSERT(mm_mmu_access(mmu, 31, NULL) == 15, "");

  mm_mmu_unmap(mmu, &mmu->pages[1]);
  mm_mmu_map(mmu, &mmu->pages[2], 0);

  ASSERT(mm_mmu_access(mmu, 32, NULL) == 0, "");
  ASSERT(mm_mmu_access(mmu, 33, NULL) == 1, "");
  ASSERT(mm_mmu_access(mmu, 47, NULL) == 15, "");

  mm_mmu_destroy(mmu);
}

//  (virtual)        (physical)
//   ...
// 1  : 16-31       1 : 16-31
// 0  :  0-15       0 :  0-15
void test4()
{
  mm_mmu_t* mmu = mm_mmu_create(64, 32, 16, mm_nrup_algorithm);
  mm_nrup_init(mmu->pages_count);

  // (v_page)     (p_frame)
  //    1             0
  //    0             1
  LOGERR("access = `%u`", mm_mmu_access(mmu, 16, NULL));
  ASSERT(mm_mmu_access(mmu, 16, NULL) == 0, "");
  ASSERT(mm_mmu_access(mmu, 17, NULL) == 1, "");
  ASSERT(mm_mmu_access(mmu, 18, NULL) == 2, "");

  ASSERT(mmu->free_pageframes_count == 1, "");
  ASSERT(mmu->free_pageframes[0] == 0, "");
  ASSERT(mmu->free_pageframes[1] == 1, "");

  ASSERT(mm_mmu_access(mmu, 0, NULL) == 16, "");
  ASSERT(mm_mmu_access(mmu, 1, NULL) == 17, "");
  ASSERT(mm_mmu_access(mmu, 2, NULL) == 18, "");

  ASSERT(mmu->free_pageframes_count == 0, "");
  ASSERT(mmu->free_pageframes[0] == 0, "");
  ASSERT(mmu->free_pageframes[1] == 0, "");

  mm_nrup_destroy(mmu->pages_count);
  mm_mmu_destroy(mmu);
}

//  (virtual)        (physical)
// 3  : 48-65
// 2  : 32-47
// 1  : 16-31       1 : 16-31
// 0  :  0-15       0 :  0-15
void test5()
{
  mm_mmu_t* mmu = mm_mmu_create(64, 32, 16, mm_nrup_algorithm);
  mm_nrup_init(mmu->pages_count);

  ASSERT(mmu->free_pageframes_count == 2, "");

  // (v_page)     (p_frame)
  //    2             -
  //    1             0
  //    0             1
  ASSERT(mm_mmu_access(mmu, 16, NULL) == 0, "");
  ASSERT(mm_mmu_access(mmu, 0, NULL) == 16, "");
  ASSERT(mmu->free_pageframes_count == 0, "");

  mm_mmu_access(mmu, 32, NULL);
  ASSERT(mmu->pages[2].p == 1,
         "VPage 2 has to be in the set of those which are present in phys");
  // both vpage 1 and 2 were in the same class for the nru algorithm
  //    --> rand() takes place as the tie-braker
  ASSERT(mmu->pages[0].p == 0 || mmu->pages[1].p == 0,
         "Maybe vpage 1 or vpage 2 was substituted");

  mm_mmu_access(mmu, 16, NULL);
  ASSERT(mmu->pages[1].p == 1, "");
  ASSERT(mmu->pages[0].p == 0 || mmu->pages[2].p == 0, "");

  mm_mmu_access(mmu, 0, NULL);
  ASSERT(mmu->pages[0].p == 1, "");
  ASSERT(mmu->pages[1].p == 0 || mmu->pages[2].p == 0, "");

  mm_nrup_destroy(mmu->pages_count);
  mm_mmu_destroy(mmu);
}

/* void test6() */
/* { */
/*   mm_mmu_t* mmu = mm_mmu_create(64, 32, 16, NULL); */
/*   unsigned mb; */

/*   mm_mmu_access(mmu, 16, &mb); */
/*   ASSERT(mb == 0, ""); */

/*   mb = 255; */
/*   mm_mmu_access(mmu, 17, &mb); */
/*   ASSERT(mb == 255, ""); */

/*   mm_mmu_access(mmu, 32, &mb); */
/*   ASSERT(mb == 1, ""); */

/*   mm_mmu_destroy(mmu); */
/* } */

//  (virtual)        (physical)
// 3  : 48-65
// 2  : 32-47
// 1  : 16-31       1 : 16-31
// 0  :  0-15       0 :  0-15
void test7()
{
  mm_mmu_t* mmu = mm_mmu_create(64, 32, 16, mm_fifo_algorithm);
  // TODO maybe pageframe, actually?
  mm_fifo_init(mmu->pages_count);

  // (v_page)     (p_frame)
  //    2             -
  //    1             0
  //    0             1
  ASSERT(mm_mmu_access(mmu, 16, NULL) == 0, ""); // 1st mapping (fifo: 1)
  ASSERT(mm_mmu_access(mmu, 0, NULL) == 16, ""); // 2nd mapping (fifo: 1-0)
  ASSERT(mmu->free_pageframes_count == 0, "");

  mm_mmu_access(mmu, 32, NULL); // 3rd mapping - page subst occurs
                                // takes the place of 1
                                // fifo: 0-2
  ASSERT(mmu->pages[0].p == 1,
         "vpage 0 must be present as it was the second to be mapped");
  ASSERT(mmu->pages[1].p == 0, "vpage 1 must not be present");
  ASSERT(mmu->pages[2].p == 1,
         "VPage 2 has to be in the set of those which are present in phys");

  /* mm_mmu_access(mmu, 16, NULL); */
  /* ASSERT(mmu->pages[1].p == 1, ""); */
  /* ASSERT(mmu->pages[0].p == 0 || mmu->pages[2].p == 0, ""); */

  /* mm_mmu_access(mmu, 0, NULL); */
  /* ASSERT(mmu->pages[0].p == 1, ""); */
  /* ASSERT(mmu->pages[1].p == 0 || mmu->pages[2].p == 0, ""); */

  mm_fifo_destroy(mmu->pages_count);
  mm_mmu_destroy(mmu);
}

int main()
{
  TEST(test1, "Initialization");
  TEST(test2, "Mapping and unmapping");
  TEST(test3, "Access to mapped area");
  TEST(test4, "Access to unmapped area w/ phys space");
  TEST(test5, "nrup - Access to unmapped area w/out phys space");
  /* TEST(test6, "Mapping notice during access"); */
  TEST(test7, "fifo - Access to unmapped area w/out phys space");

  return 0;
}
