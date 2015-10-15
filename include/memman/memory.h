#ifndef MEMMAN__MEMORY_H
#define MEMMAN__MEMORY_H

#include "memman/common.h"

/**
 * Representation of a memory (virtual or physical).
 * Encapsulates a file and operations on it
 */
typedef struct mm_memory_t {
  mm_memory_type_e type;
  FILE* file;
  unsigned size;
  char fname[MM_MEM_FNAME_SIZE]; 
  unsigned char* buf;
} mm_memory_t;

mm_memory_t* mm_memory_create(unsigned size, mm_memory_type_e type);
void mm_memory_init_file(mm_memory_t* mem);
void mm_memory_destroy(mm_memory_t* mem);
void mm_memory_assign(mm_memory_t* mem, unsigned base, unsigned length,
                      int value);

#endif
