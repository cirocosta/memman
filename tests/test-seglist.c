#include "memman/common.h"
#include "memman/seglist.h"
#include "memman/process.h"

void test1()
{
  mm_seglist_t* list = mm_seglist_create(10, MM_ALG_FREE_FF);

  // no process assigned but only a big hole
  ASSERT(list->processes->next == NULL, "");
  ASSERT(list->holes->next != NULL, "");
  ASSERT(list->holes->next->next == NULL, "");

  // this should be more highlevel
  ASSERT(((mm_segment_t*)list->holes->next->data)->length == 10,
         "The entire list is freed");
  ASSERT(((mm_segment_t*)list->holes->next->data)->start == 0,
         "It starts at the first pos");

  mm_seglist_destroy(list);
}

void test2()
{
  mm_seglist_t* list = mm_seglist_create(10, MM_ALG_FREE_FF);

  // initializing a process that takes 5 bytes
  mm_process_t* process = mm_process_create();
  process->b = 5;

  mm_seglist_add_process(list, process);

  ASSERT(((mm_segment_t*)list->processes->next->data)->length == 5,
         "Process length matches");
  ASSERT(((mm_segment_t*)list->processes->next->data)->start == 0,
         "Process position matches");

  ASSERT(((mm_segment_t*)list->holes->next->data)->start == 5,
         "Hole starts after the first process");
  ASSERT(((mm_segment_t*)list->holes->next->data)->length == 5,
         "Hole has the expected length");

  mm_seglist_destroy(list);
}

int main(int argc, char* argv[])
{
  TEST(test1, "No processes");
  TEST(test2, "First-Firt: first process assignment");

  return 0;
}
