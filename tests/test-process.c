#include "memman/common.h"
#include "memman/process.h"

/**
 * mm_process_parse turns:
 *  `t0 name tf b 1*(pn tn)`
 * into a process.
 */

void test1()
{
  const char* src = "0 process0 2 10 1 1 2 2";
  mm_process_t* process = mm_process_parse(src);

  ASSERT(process->t0 == 0, "");
  STRNCMP(process->pname, "process0");
  ASSERT(process->tf == 2, "");
  ASSERT(process->b == 10, "");
  ASSERT(process->access_count == 2, "");
  ASSERT(process->access[0].position == 1, "");
  ASSERT(process->access[0].time == 1, "");
  ASSERT(process->access[1].position == 2, "");
  ASSERT(process->access[1].time == 2, "");

  mm_process_destroy(process);
}

int main(int argc, char *argv[])
{
  TEST(test1);
  
  return 0;
}
