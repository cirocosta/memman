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
  // TODO what about the segments?
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

static mm_dllist_t* _search_end(mm_dllist_t* list, unsigned pos)
{
  mm_segment_t* tmp;

  do {
    if (!list->data)
      continue;

    tmp = (mm_segment_t*)list->data;

    if (tmp->start + tmp->length == pos)
      return list;

  } while ((list = list->next));

  return NULL;
}

// pxp ==> pfp
static void _free_pxp(mm_seglist_t* sl, mm_dllist_t* lhs, mm_dllist_t* x,
                      mm_dllist_t* rhs)
{
}

// pxf ==> pf
static void _free_pxf(mm_seglist_t* sl, mm_dllist_t* lhs, mm_dllist_t* x,
                      mm_dllist_t* rhs)
{
}

// fxp ==> fp
static void _free_fxp(mm_seglist_t* sl, mm_dllist_t* lhs, mm_dllist_t* x,
                      mm_dllist_t* rhs)
{
}

// fxf ==> f
static void _free_fxf(mm_seglist_t* sl, mm_dllist_t* lhs, mm_dllist_t* x,
                      mm_dllist_t* rhs)
{
}

// nxp ==> f
static void _free_nxp(mm_seglist_t* sl, mm_dllist_t* lhs, mm_dllist_t* x,
                      mm_dllist_t* rhs)
{
  mm_segment_t* seg_x = (mm_segment_t*)x->data;

  mm_dllist_remove(x);
  mm_process_destroy(seg_x->process);
  seg_x->process = NULL;

  mm_dllist_append(sl->holes, x);
}

// nxf ==> f
static void _free_nxf(mm_seglist_t* sl, mm_dllist_t* lhs, mm_dllist_t* x,
                      mm_dllist_t* rhs)
{
  mm_segment_t* seg_rhs = (mm_segment_t*)rhs->data;
  mm_segment_t* seg_x = (mm_segment_t*)x->data;

  seg_rhs->start -= seg_x->length;
  seg_rhs->length += seg_x->length;

  mm_dllist_remove(x);
  mm_segment_destroy(seg_x);
}

void mm_seglist_free_process(mm_seglist_t* list, mm_segment_t* process)
{
  mm_segment_t* tmp_seg = NULL;
  mm_dllist_t* proc_left = NULL;
  mm_dllist_t* proc_right = NULL;
  mm_dllist_t* free_left = NULL;
  mm_dllist_t* free_right = NULL;

  unsigned right_pos = process->start + process->length;
  mm_dllist_t* pl = _search_start(list->processes, process->start);

  // has a process at its right
  if (pl->next && (((mm_segment_t*)pl->next->data)->start == right_pos)) {
    proc_right = pl->next;
  } else {
    free_right = _search_start(list->holes, right_pos);
  }

  // has process at left
  if (pl->prev) {
    tmp_seg = (mm_segment_t*)pl->prev->data;

    if (tmp_seg && tmp_seg->start + tmp_seg->length == process->start)
      proc_left = pl->prev;
  }

  if (!proc_left) {
    free_left = _search_end(list->holes, process->start);
  }

  if (proc_left && proc_right) {
    _free_pxp(list, proc_left, pl, proc_right);
  } else if (proc_left && free_right) {
    _free_pxf(list, proc_left, pl, free_right);
  } else if (free_left && proc_right) {
    _free_fxp(list, free_left, pl, proc_right);
  } else if (free_left && free_right) {
    _free_fxf(list, free_left, pl, free_right);
  } else if (!(free_left || proc_left) && free_right) {
    _free_nxf(list, NULL, pl, free_right);
  } else if (!(free_left || proc_left) && proc_right) {
    _free_nxp(list, NULL, pl, proc_right);
  }

  // do the job

#if 0
  if (free_left && free_right) { // [F][X][F]
    // FIXME TEST THIS CASE
    ((mm_segment_t*)free_right->data)->start -= process->length;
    ((mm_segment_t*)free_right->data)->length += process->length;

    ((mm_segment_t*)free_left->data)->length +=
        ((mm_segment_t*)free_right->data)->length;
    mm_dllist_remove(free_right);
    mm_dllist_remove(pl);

    return;
  }
#endif
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
