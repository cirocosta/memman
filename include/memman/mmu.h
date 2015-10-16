#ifndef MEMMAN__MMU_H
#define MEMMAN__MMU_H

#include "memman/common.h"
#include "memman/vpage.h"

#include <limits.h>
#include <stdint.h>
#include <math.h>

struct mm_mmu_t;
typedef mm_vpage_t* (*mm_mmu_pr_alg)(struct mm_mmu_t* mmu, uint8_t page);
typedef void (*mm_mmu_map_cb)(mm_vpage_t*, void* data);

typedef struct mm_mmu_t {
  unsigned pages_count;
  unsigned pageframes_count;
  mm_vpage_t* pages;

  mm_mmu_pr_alg replacement_alg;

  uint8_t offset_size_bits;
  uint8_t page_size_bits;
  uint8_t frame_size_bits;
  uint8_t offset_mask;

  unsigned char* free_pageframes; // 0 ==> not free. 1 ==> fre
  unsigned free_pageframes_count;
} mm_mmu_t;

// public
mm_mmu_t* mm_mmu_create(unsigned vsize, unsigned psize,
                        unsigned page_frame_size,
                        mm_mmu_pr_alg replacement_alg);

void mm_mmu_destroy(mm_mmu_t* mmu);
//                          mmu           pos      value-passing mapped base
unsigned mm_mmu_access(mm_mmu_t* mmu, unsigned position, unsigned* mb);

// private
unsigned mm_mmu_map_free_pageframe(mm_mmu_t* mmu, mm_vpage_t* page);
void mm_mmu_set_replacememt_alg(mm_mmu_t* mmu, mm_mmu_pr_alg replacement_alg);
void mm_mmu_reset_bits(mm_mmu_t* mmu);
unsigned mm_mmu_map(mm_mmu_t* mmu, mm_vpage_t* vpage, unsigned phys_page);
void mm_mmu_unmap(mm_mmu_t* mmu, mm_vpage_t* vpage);

#endif
