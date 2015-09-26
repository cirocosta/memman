#ifndef MEMMAN__MEMORY_H
#define MEMMAN__MEMORY_H

#include "memman/common.h"

typedef struct mm_memory_t {
  mm_memory_type type;
  FILE* file;
  unsigned size;
  char fname[MM_MEM_FNAME_SIZE]; 
} mm_memory_t;

mm_memory_t* mm_memory_create(unsigned size, mm_memory_type type);
void mm_memory_init_file(mm_memory_t* mem);
void mm_memory_destroy(mm_memory_t* mem);
void mm_memory_assign(mm_memory_t* mem, unsigned base, unsigned length,
                      int value);

#endif
