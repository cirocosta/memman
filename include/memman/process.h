#ifndef MEMMAN__PROCES_H
#define MEMMAN__PROCES_H

#include "memman/common.h"

typedef struct mm_process_access_t {
  unsigned position;
  unsigned time;
} mm_process_access_t;

/**
 * Representation of a process. It agglomerates its creation, destruction
 * and all accesses that it'll perform during its lifetime.
 */
typedef struct mm_process_t {
  unsigned b;
  unsigned t0;
  unsigned tf;
  char pname[MM_MAX_NAME];
  mm_process_access_t access[MM_MAX_ACCESS];
  unsigned access_count;
  int8_t pid;
} mm_process_t;

static mm_process_t zeroed_process_t = { 0 };

mm_process_t* mm_process_create();
void mm_process_destroy(mm_process_t* process);
mm_process_t* mm_process_parse(const char* src);

#endif
