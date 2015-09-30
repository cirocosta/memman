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
    
    // FIXME create a test for this
    if (list->segment)
      mm_segment_destroy(list->segment);

    FREE(list);
    list = next;
  }
}

void mm_dllist_append(mm_dllist_t* a, mm_dllist_t* b)
{
  b->prev = a;
  b->next = a->next;
  a->next = b;
}

// FIXME deprecated
mm_dllist_t* mm_dllist_insert_after(mm_dllist_t* a, mm_segment_t* data)
{
  mm_dllist_t* b = mm_dllist_create(data);
  mm_dllist_append(a,b);

  return b;
}

void mm_dllist_remove(mm_dllist_t* list)
{
  mm_dllist_t* prev = list->prev;
  mm_dllist_t* next = list->next;

  if (next)
    list->next->prev = prev;
  if (prev)
    list->prev->next = next;

  list->next = list->prev = NULL;
}
