#ifndef MEMMAN__DLLIST_H
#define MEMMAN__DLLIST_H

#include "memman/common.h"

/**
 * SETUP:
 *  - L is a list of lists
 *  - L->head points to the first element
 *  - x is an element from the list
 *
 * INVARIANTS:
 *  - x is a list itself
 *  - x->prev == NIL ==> x is head
 *  - x->next == NIL ==> x is tail
 *  - L->head == NIL ==> list is empty
 */

typedef struct mm_dllist_t {
  struct mm_dllist_t* next;
  struct mm_dllist_t* prev;
  void* data;
} mm_dllist_t;

mm_dllist_t* mm_dllist_create(void* data);
void mm_dllist_destroy(mm_dllist_t* list);
mm_dllist_t* mm_dllist_insert_after(mm_dllist_t* a, void* data);
void mm_dllist_remove(mm_dllist_t* list);

#endif
