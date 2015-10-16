#ifndef MEMMAN__QUEUE_H
#define MEMMAN__QUEUE_H

#include "memman/common.h"
#include <stdlib.h>

typedef struct mm_queue_t {
  void** elems;
  unsigned f;
  unsigned r;
  size_t n;
  size_t length;
} mm_queue_t;

mm_queue_t* mm_queue_create(unsigned count);
void mm_queue_destroy(mm_queue_t* q);
void mm_queue_remove(mm_queue_t* q);
unsigned mm_queue_insert(mm_queue_t* q, void* elem);

typedef int (*mm_queue_sort_func)(const void* a, const void* b);

void mm_queue_sort(mm_queue_t* q, mm_queue_sort_func func);
int mm_queue_empty(mm_queue_t* q);
void* mm_queue_back(mm_queue_t* q);
void* mm_queue_front(mm_queue_t* q);

#endif
