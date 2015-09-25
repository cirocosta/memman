#ifndef MEMMAN__MANAGER_H
#define MEMMAN__MANAGER_H

#include "memman/common.h"
#include "memman/process.h"

typedef struct mm_manager_t {
  unsigned physical;
  unsigned virtual;

  mm_process_t** processes;
  unsigned process_count;
} mm_manager_t;

static mm_manager_t zeroed_manager_t = { 0 };

mm_manager_t* mm_manager_create();
void mm_manager_destroy(mm_manager_t*);
mm_manager_t* mm_manager_parse_file(const char* fname);

#endif
