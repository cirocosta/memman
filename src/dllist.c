#include "memman/dllist.h"

mm_dllist_t* mm_dllist_create(mm_segment_t* data)
{
  mm_dllist_t* list = malloc(sizeof(*list));
  PASSERT(list, MM_ERR_MALLOC);

  list->next = NULL;
  list->prev = NULL;
  list->segment = data;

  return list;
}

void mm_dllist_destroy(mm_dllist_t* list)
{
  mm_dllist_t* next;

  while (list) {
    next = list->next;
    if (list->segment)
      mm_segment_destroy(list->segment);
    FREE(list);
    list = next;
  }
}

void mm_dllist_append(mm_dllist_t* a, mm_dllist_t* b)
{
  if (a->next)
    a->next->prev = b;
  b->prev = a;
  b->next = a->next;
  a->next = b;
}

// FIXME deprecated
mm_dllist_t* mm_dllist_insert_after(mm_dllist_t* a, mm_segment_t* data)
{
  mm_dllist_t* b = mm_dllist_create(data);
  mm_dllist_append(a, b);

  return b;
}

void mm_dllist_remove(mm_dllist_t* list)
{
  mm_dllist_t* prev = list->prev;
  mm_dllist_t* next = list->next;

  if (next)
    next->prev = prev;
  if (prev)
    prev->next = next;

  list->next = NULL;
  list->prev = NULL;
}

void mm_dllist_show(mm_dllist_t* list)
{
  while (list) {
    mm_segment_show(list->segment);
    list = list->next;
  }

  fprintf(stderr, "NIL\n");
}
