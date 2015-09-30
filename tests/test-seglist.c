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
  ASSERT((list->holes->next->segment)->length == 10,
         "The entire list is freed");
  ASSERT((list->holes->next->segment)->start == 0,
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

  ASSERT((list->processes->next->segment)->length == 5,
         "Process length matches");
  ASSERT((list->processes->next->segment)->start == 0,
         "Process position matches");

  ASSERT((list->holes->next->segment)->start == 5,
         "Hole starts after the first process");
  ASSERT((list->holes->next->segment)->length == 5,
         "Hole has the expected length");

  mm_seglist_destroy(list);
}

void test3()
{
  mm_seglist_t* list = mm_seglist_create(12, MM_ALG_FREE_FF);

  // initializing 2 processes, each with 3 bytes
  mm_process_t* process1 = mm_process_create();
  mm_process_t* process2 = mm_process_create();

  process1->b = 3;
  process2->b = 3;

  mm_seglist_add_process(list, process1);
  mm_seglist_add_process(list, process2);

  ASSERT((list->processes->next->segment)->start == 0, "");
  ASSERT((list->processes->next->segment)->process == process1, "");

  ASSERT((list->processes->next->next->segment)->start == 3, "");
  ASSERT((list->processes->next->next->segment)->process == process2, "");

  ASSERT((list->holes->next->segment)->start == 6,
         "Hole starts after the second process");
  ASSERT((list->holes->next->segment)->length == 6, "");

  mm_seglist_destroy(list);
}

void test4()
{
  mm_seglist_t* list = mm_seglist_create(10, MM_ALG_FREE_FF);
  mm_segment_t* proc_seg;

  // initializing a process that takes 5 bytes
  mm_process_t* process = mm_process_create();
  process->b = 5;
  // PROC S->nil
  // FREE S->|0,10|->nil

  proc_seg = mm_seglist_add_process(list, process);
  // PROC S->|0,5|->nil
  // FREE S->|5,5|->nil
  // OVERALL  [P,0,5][F,5,5]
  //        =====>
  // OVERALL  [F,0,10]

  mm_seglist_free_process(list, proc_seg);
  // PROC S->nil
  // FREE S->|0,10|->nil

  ASSERT((list->holes->next->segment)->start == 0, "");
  ASSERT((list->holes->next->segment)->length == 10, "");

  mm_seglist_destroy(list);
}

void test5()
{
  mm_seglist_t* list = mm_seglist_create(12, MM_ALG_FREE_FF);

  // initializing 2 processes, each with 3 bytes
  mm_process_t* process1 = mm_process_create();
  mm_process_t* process2 = mm_process_create();
  mm_segment_t* proc1_seg;

  process1->b = 3;
  process2->b = 3;

  proc1_seg = mm_seglist_add_process(list, process1);
  mm_seglist_add_process(list, process2);

  //                 p1    p2
  // OVERALL    [][P,0,3][P,3,3][F,6,6]
  //                ===free(p1)==>
  // OVERALL    [][F,0,3][P,3,3][F,6,6]
  mm_seglist_free_process(list, proc1_seg);

  ASSERT((list->holes->next->segment)->start == 0, "");
  ASSERT((list->holes->next->segment)->length == 3, "");
  ASSERT((list->holes->next->next->segment)->start == 6, "");
  ASSERT((list->holes->next->next->segment)->length == 6, "");

  ASSERT((list->processes->next->segment)->start == 3, "");
  ASSERT((list->processes->next->segment)->length == 3, "");

  mm_seglist_destroy(list);
}

int main(int argc, char* argv[])
{
  TEST(test1, "No processes");
  /* TEST(test2, "First-Firt: first process assignment"); */
  /* TEST(test3, "First-Firt: second process assignment"); */
  /* TEST(test4, "Freeing single-process"); */
  /* TEST(test5, "Freeing The first process in a 2proc scenario"); */

  return 0;
}
