#ifndef MEMMAN__DLLIST_H
#define MEMMAN__DLLIST_H

#include "memman/common.h"

/**
 * SETUP:
 *  - L->head points to the first element
 *  - L->nil is the sentinel
 *  - x is an element from the list
 *
 * INVARIANTS:
 *  - x->prev == NIL ==> x is head
 *  - x->next == NIL ==> x is tail
 *  - L->head == NIL ==> list is empty
 */

typedef struct mm_dllist_t {
  struct mm_dllist_t* next;
  struct mm_dllist_t* prev;
  void* data;
} mm_dllist_t;

mm_dllist_t* mm_dllist_create(void* data)
{
  mm_dllist_t* list = malloc(sizeof(*list));
  PASSERT(list, MM_ERR_MALLOC);

  list->next = NULL;
  list->prev = NULL;
  list->data = data;

  return list;
}

void mm_dllist_destroy(mm_dllist_t* list)
{
  mm_dllist_t* initial = list;
  mm_dllist_t* next;

  while ((next = list->next)) {
    free(next);
    list = next;
  }

  free(initial);
}

/* void mm_dllist_search(mm_dllist_t* list, void* k); */

mm_dllist_t* mm_dllist_insert_after(mm_dllist_t* a, void* data)
{
  mm_dllist_t* b = mm_dllist_create(data);

  b->prev = a;
  b->next = a->next;
  a->next = b;

  return b;
}


/* void mm_dllist_remove(mm_dllist_t* list, void* k); */

#endif
