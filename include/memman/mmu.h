#ifndef MEMMAN__MMU_H
#define MEMMAN__MMU_H

#include "memman/common.h"
#include "memman/vpage.h"

#include <limits.h>
#include <stdint.h>
#include <math.h>

typedef mm_vpage_t* (*mm_mmu_pr_alg)(mm_vpage_t* pages, unsigned pages_count);

typedef struct mm_mmu_t {
  mm_vpage_t* pages;
  unsigned pages_count;

  unsigned unmapped_pages;
  mm_mmu_pr_alg replacement_alg;

  uint8_t offset_mask;
  uint8_t offset_size_bits;
  uint8_t page_size_bits;
  uint8_t frame_size_bits;
} mm_mmu_t;

mm_mmu_t* mm_mmu_create(unsigned vsize, unsigned psize,
                        unsigned page_frame_size,
                        mm_mmu_pr_alg replacement_alg);
void mm_mmu_destroy(mm_mmu_t* mmu);

void mm_mmu_reset_bits(mm_mmu_t* mmu);
void mm_mmu_map(mm_mmu_t* mmu, mm_vpage_t* vpage, unsigned phys_page);
inline static void mm_mmu_unmap(mm_mmu_t* mmu, mm_vpage_t* vpage)
{
  vpage->p = 0;
}
unsigned mm_mmu_access(mm_mmu_t* mmu, unsigned position);

#endif
