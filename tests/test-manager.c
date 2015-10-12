#include "memman/manager.h"

void test1()
{
  const char* fname = "tests/assets/trace.txt";
  mm_manager_t* manager = mm_manager_parse_file(fname);

  ASSERT(manager->physical && manager->virtual,
         "Must have created physical and virtual memories");
  /* ASSERT(manager->physical->size == 100, ""); */
  /* ASSERT(manager->virtual->size == 100, ""); */
  ASSERT(manager->process_count == 4, "");

  ASSERT(manager->processes[0]->t0 == 0, "");
  STRNCMP(manager->processes[0]->pname, "process0");
  ASSERT(manager->processes[0]->access_count == 2, "");
  ASSERT(manager->processes[0]->access[0].position == 1, "");
  ASSERT(manager->processes[0]->access[0].time == 1, "");

  mm_manager_destroy(manager);
}

int main(int argc, char* argv[])
{
  TEST(test1, "Loads a file and parses it");

  return 0;
}
