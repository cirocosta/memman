#include "memman/queue.h"

mm_queue_t* mm_queue_create(unsigned count)
{
  mm_queue_t* queue = malloc(sizeof(*queue));
  PASSERT(queue, "Couldn't allocate memory properly");

  queue->f = 0;
  queue->r = 0;
  queue->length = 0;
  queue->n = count;

  queue->elems = calloc(count, sizeof(*queue->elems));
  PASSERT(queue->elems, MM_ERR_MALLOC);

  return queue;
}

void mm_queue_destroy(mm_queue_t* q)
{
  FREE(q->elems);
  FREE(q);
}

void mm_queue_sort(mm_queue_t* q, mm_queue_sort_func func)
{
  qsort(q->elems, q->length, sizeof(void*), func);
}

int mm_queue_empty(mm_queue_t* q) { return q->f == q->r; }

void* mm_queue_back(mm_queue_t* q)
{
  if (!mm_queue_empty(q))
    return q->elems[q->r];
  return NULL;
}

void* mm_queue_front(mm_queue_t* q)
{
  if (!mm_queue_empty(q))
    return q->elems[q->f];
  return NULL;
}

unsigned mm_queue_insert(mm_queue_t* q, void* elem)
{
  unsigned pos = q->r;

  ASSERT(q->length + 1 < q->n, "Insertion would exceed maximum size");
  q->elems[q->r] = elem;

  if (q->r == q->n - 1)
    q->r = 0;
  else
    q->r++;

  q->length++;

  return pos;
}

void mm_queue_remove(mm_queue_t* q)
{
  if (q->f == q->n - 1)
    q->f = 0;
  else
    q->f++;

  q->length--;
}
