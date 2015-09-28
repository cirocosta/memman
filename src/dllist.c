#include "memman/dllist.h"

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
  mm_dllist_t* next;

  while (list) {
    next = list->next;
    FREE(list);
    list = next;
  }
}

mm_dllist_t* mm_dllist_insert_after(mm_dllist_t* a, void* data)
{
  mm_dllist_t* b = mm_dllist_create(data);

  b->prev = a;
  b->next = a->next;
  a->next = b;

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
