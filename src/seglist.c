#include "memman/seglist.h"

mm_seglist_t* mm_seglist_create(unsigned size, mm_algorithms_e algorithm)
{
  mm_segment_t* hole_seg = mm_segment_create(0, size);
  mm_seglist_t* list = malloc(sizeof(*list));
  PASSERT(list, MM_ERR_MALLOC);

  list->size = size;
  list->processes = mm_dllist_create(NULL);
  list->holes = mm_dllist_create(NULL);

  mm_dllist_insert_after(list->holes, hole_seg);

  list->algorithm_type = algorithm;
  switch (algorithm) {
    case MM_ALG_FREE_FF:
      list->algorithm = &mm_freemem_ff;
      break;
    case MM_ALG_FREE_NF:
    case MM_ALG_FREE_QF:
      LOGERR("Unsupported algorithm");
      exit(EXIT_FAILURE);
      break;
  }

  return list;
}

void mm_seglist_destroy(mm_seglist_t* list)
{
  mm_dllist_destroy(list->holes);
  mm_dllist_destroy(list->processes);
  FREE(list);
}

static mm_dllist_t* _search_b4_start(mm_dllist_t* list, mm_segment_t* seg)
{
  while (list) {
    if (!list->next)
      return list;
    if (((mm_segment_t*)list->next->data)->start >= seg->start)
      return list;
    list = list->next;
  }

  return list;
}

static mm_dllist_t* _search_start(mm_dllist_t* list, unsigned pos)
{
  do {
    if (!list->data)
      continue;

    if (((mm_segment_t*)list->data)->start == pos)
      return list;
  } while ((list = list->next));

  return NULL;
}

void mm_seglist_free_process(mm_seglist_t* list, mm_segment_t* process)
{
  // |PROC|FREE...| ==> |FREE....|
  unsigned pos = process->start + process->length;
  mm_dllist_t* pl = _search_start(list->processes, process->start);
  mm_segment_t* seg = (mm_segment_t*)_search_start(list->holes, pos)->data;

  seg->start -= process->length;
  seg->length += process->length;

  mm_dllist_remove(pl);
}

mm_segment_t* mm_seglist_add_process(mm_seglist_t* list, mm_process_t* process)
{
  mm_segment_t* free_seg = list->algorithm(list->holes->next, process->b);
  mm_segment_t* process_seg = mm_segment_create(free_seg->start, process->b);
  process_seg->process = process;

  mm_dllist_t* proc_b4 = _search_b4_start(list->processes, process_seg);
  mm_dllist_insert_after(proc_b4, process_seg);

  free_seg->length -= process->b;
  free_seg->start += process->b;

  return process_seg;
}
