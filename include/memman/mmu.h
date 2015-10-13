#ifndef MEMMAN__MMU_H
#define MEMMAN__MMU_H

#include "memman/common.h"
#include <limits.h>
#include <stdint.h>
#include <math.h>

typedef struct mm_vpage_t {
  unsigned char r : 1; // read
  unsigned char m : 1; // modified
  unsigned char p : 1; // present
  unsigned phys_page;  // physical page (mapping)
} mm_vpage_t;

static const mm_vpage_t mm_vpage_zeroed = { 0 };

typedef struct mm_mmu_t {
  mm_vpage_t* pages;
  unsigned pages_count;
  void* replacement_alg;

  uint8_t offset_mask;
  uint8_t offset_size_bits;
  uint8_t page_size_bits;
  uint8_t frame_size_bits;
} mm_mmu_t;

mm_mmu_t* mm_mmu_create(unsigned vsize, unsigned psize,
                        unsigned page_frame_size);
void mm_mmu_destroy(mm_mmu_t* mmu);

void mm_mmu_reset_bits(mm_mmu_t* mmu);
void mm_mmu_map(mm_mmu_t* mmu, unsigned vpage, unsigned phys_page);
void mm_mmu_unmap(mm_mmu_t* mmu, unsigned vpage);
unsigned mm_mmu_access(mm_mmu_t* mmu, unsigned position);

#endif
