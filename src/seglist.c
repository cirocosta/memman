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

/**
 * Searches for the list member whose position is
 * less than our segment's start
 */
static mm_dllist_t* _search_b4_start(mm_dllist_t* list, unsigned pos)
{
  while (list) {
    if (!list->next)
      return list;
    if ((list->next->segment)->start >= pos)
      return list;
    list = list->next;
  }

  return list;
}

/**
 * Search for the exact list member whose
 * start position equals pos
 */
static mm_dllist_t* _search_start(mm_dllist_t* list, unsigned pos)
{
  do {
    if (!list->segment)
      continue;

    if ((list->segment)->start == pos)
      return list;
  } while ((list = list->next));

  return NULL;
}

static mm_dllist_t* _search_end(mm_dllist_t* list, unsigned pos)
{
  mm_segment_t* tmp;

  do {
    if (!list->segment)
      continue;

    tmp = list->segment;

    if (tmp->start + tmp->length == pos)
      return list;

  } while ((list = list->next));

  return NULL;
}

// pxp ==> pfp
static void _free_pxp(mm_seglist_t* sl, mm_dllist_t* lhs, mm_dllist_t* x,
                      mm_dllist_t* rhs)
{
  mm_dllist_remove(x); // won't delete, recycle
  mm_process_destroy(x->segment->process);
  x->segment->process = NULL;

  mm_dllist_append(_search_b4_start(sl->holes, x->segment->start + 1), x);
}

// pxf ==> pf
static void _free_pxf(mm_seglist_t* sl, mm_dllist_t* lhs, mm_dllist_t* x,
                      mm_dllist_t* rhs)
{
  rhs->segment->length += x->segment->length;
  rhs->segment->start -= x->segment->length;

  mm_dllist_remove(x);
  mm_dllist_destroy(x);
}

// fxp ==> fp
static void _free_fxp(mm_seglist_t* sl, mm_dllist_t* lhs, mm_dllist_t* x,
                      mm_dllist_t* rhs)
{
  lhs->segment->length += x->segment->length;

  mm_dllist_remove(x);
  mm_dllist_destroy(x);
}

// fxf ==> f
static void _free_fxf(mm_seglist_t* sl, mm_dllist_t* lhs, mm_dllist_t* x,
                      mm_dllist_t* rhs)
{
  lhs->segment->length += rhs->segment->length;
  lhs->segment->length += x->segment->length;

  mm_dllist_remove(x);
  mm_dllist_destroy(x);

  mm_dllist_remove(rhs);
  mm_dllist_destroy(rhs); // err
}

// nxp ==> f
static void _free_nxp(mm_seglist_t* sl, mm_dllist_t* lhs, mm_dllist_t* x,
                      mm_dllist_t* rhs)
{
  mm_dllist_remove(x); // NIL <- X -> NIL
  mm_process_destroy(x->segment->process);
  x->segment->process = NULL;

  mm_dllist_append(sl->holes, x);
}

// nxf ==> f
static void _free_nxf(mm_seglist_t* sl, mm_dllist_t* lhs, mm_dllist_t* x,
                      mm_dllist_t* rhs)
{

  rhs->segment->start -= x->segment->length;
  rhs->segment->length += x->segment->length;

  mm_dllist_remove(x);
  mm_dllist_destroy(x);
}

mm_segment_t* mm_seglist_search_process(mm_seglist_t* list,
                                        mm_process_t* process)
{
  mm_dllist_t* proclist = list->processes;

  while ((proclist = proclist->next)) {
    if (proclist->segment->process == process)
      return proclist->segment;
  }

  return NULL;
}

void mm_seglist_free_process(mm_seglist_t* list, mm_segment_t* process)
{
  mm_dllist_t* proc_left = NULL;
  mm_dllist_t* proc_right = NULL;
  mm_dllist_t* free_left = NULL;
  mm_dllist_t* free_right = NULL;

  unsigned right_pos = process->start + process->length;
  mm_dllist_t* pl = _search_start(list->processes, process->start);
  ASSERT(pl, "pl must be in memory");

  // has a process at its right
  if (pl->next && ((pl->next->segment)->start == right_pos)) {
    proc_right = pl->next;
  } else { // may have a free at right
    free_right = _search_start(list->holes, right_pos);
  }

  // has process at left
  if (pl->prev && pl->prev->segment &&
      (pl->prev->segment->start + pl->prev->segment->length ==
       process->start)) {
    proc_left = pl->prev;
  } else { // may have a free at left
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
}

mm_segment_t* mm_seglist_add_process(mm_seglist_t* list, mm_process_t* process)
{
  mm_segment_t* free_seg = list->algorithm(list->holes->next, process->b);
  ASSERT(free_seg != NULL, "virtual memory must not be full");
  mm_segment_t* process_seg = mm_segment_create(free_seg->start, process->b);
  process_seg->process = process;

  mm_dllist_t* proc_b4 = _search_b4_start(list->processes, process_seg->start);
  mm_dllist_insert_after(proc_b4, process_seg);

  free_seg->length -= process->b;
  free_seg->start += process->b;

  return process_seg;
}
