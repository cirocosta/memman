#include "memman/simulator.h"
#include "./file-utils.h"

void test1()
{
  const char* fname = "tests/assets/trace.txt";
  mm_simulator_t* simulator = mm_simulator_parse_file(fname);

  ASSERT(simulator->physical && simulator->virtual,
         "Must have created physical and virtual memories");
  ASSERT(simulator->physical->size == 100, "");
  ASSERT(simulator->virtual->size == 100, "");
  ASSERT(simulator->process_count == 4, "");

  ASSERT(simulator->processes[0]->t0 == 0, "");
  STRNCMP(simulator->processes[0]->pname, "process0");
  ASSERT(simulator->processes[0]->access_count == 2, "");
  ASSERT(simulator->processes[0]->access[0].position == 1, "");
  ASSERT(simulator->processes[0]->access[0].time == 1, "");

  while (simulator->process_count-- > 0)
    mm_process_destroy(simulator->processes[simulator->process_count]);
  mm_simulator_destroy(simulator);
}

void test2()
{
  const char* src = "100 120\n"
                    "0 process0 2 10 1 1 2 2\n"
                    "0 process1 2 10 1 1 2 2\n"
                    "0 process2 2 10 1 1 2 2\n"
                    "0 process3 2 10 1 1 2 2\n";

  mm_simulator_t* simulator = mm_simulator_parse(src);

  ASSERT(simulator->physical && simulator->virtual,
         "Must have created physical and virtual memories");
  ASSERT(simulator->physical->size == 100, "");
  ASSERT(simulator->virtual->size == 120, "");
  ASSERT(simulator->process_count == 4, "");

  ASSERT(simulator->processes[0]->t0 == 0, "");
  STRNCMP(simulator->processes[0]->pname, "process0");
  ASSERT(simulator->processes[0]->access_count == 2, "");
  ASSERT(simulator->processes[0]->access[0].position == 1, "");
  ASSERT(simulator->processes[0]->access[0].time == 1, "");

  STRNCMP(simulator->processes[1]->pname, "process1");
  STRNCMP(simulator->processes[2]->pname, "process2");
  STRNCMP(simulator->processes[3]->pname, "process3");

  while (simulator->process_count-- > 0)
    mm_process_destroy(simulator->processes[simulator->process_count]);
  mm_simulator_destroy(simulator);
}

void test3()
{
  //                phys virt
  const char* src = "32 64\n"
                    // t0 pname tf b pn tn
                    "1 process2 3 16 0 1\n";
  mm_simulator_t* simulator = mm_simulator_parse(src);

  ASSERT(simulator->physical->size == 32, "");
  ASSERT(simulator->virtual->size == 64, "");

  mm_simulator_set_page_subst_alg(simulator, 1);
  mm_simulator_simulate(simulator);

  ASSERT(fhas_expected_value(simulator->physical->file, 0, 16, 0), "");
  ASSERT(fhas_expected_value(simulator->virtual->file, 0, 16, 0), "");

  ASSERT(simulator->page_faults == 1,
         "Only a single page fault would occur - the first one");

  mm_simulator_destroy(simulator);
}

void test4()
{
  //                phys virt
  const char* src = "32 64\n"
                    // t0 pname tf b pn tn
                    "1 process2 3 16 0 1\n";
  mm_simulator_t* simulator = mm_simulator_parse(src);
  long pf1 = 0;
  long pf2 = 0;
  long pf3 = 0;

  ASSERT(simulator->physical->size == 32, "");
  ASSERT(simulator->virtual->size == 64, "");

  mm_simulator_set_page_subst_alg(simulator, 1);
  mm_simulator_simulate(simulator);
  pf1 = simulator->page_faults;

  mm_simulator_destroy(simulator);
  simulator = mm_simulator_parse(src);
  mm_simulator_set_page_subst_alg(simulator, 2);
  mm_simulator_simulate(simulator);
  pf2 = simulator->page_faults;

  mm_simulator_destroy(simulator);
  simulator = mm_simulator_parse(src);
  mm_simulator_set_page_subst_alg(simulator, 3);
  mm_simulator_simulate(simulator);
  pf3 = simulator->page_faults;

  ASSERT(pf1 == pf2 && pf1 == pf3 && pf2 == pf3,
         "For a single access, page faults should be the same");

  mm_simulator_destroy(simulator);
}

void test5()
{
  //                phys virt
  const char* src = "32 256\n"
                    // t0 pname tf b pn tn
                    "1 process1 6 32 0 1\n"
                    "1 process2 6 32 0 1\n"
                    "1 process3 6 32 0 1\n"
                    "1 process4 6 32 0 1\n"
                    "1 process5 6 32 0 1\n"
                    "1 process6 6 32 0 1\n"
                    "1 process7 6 32 0 1\n"
                    "1 process8 6 32 0 1\n";
  mm_simulator_t* simulator = mm_simulator_parse(src);

  ASSERT(simulator->physical->size == 32, "");
  ASSERT(simulator->virtual->size == 256, "");

  mm_simulator_set_page_subst_alg(simulator, 1);
  mm_simulator_simulate(simulator);

  ASSERT(simulator->page_faults >= 8, "");

  mm_simulator_destroy(simulator);
}

int main(int argc, char* argv[])
{
  /* TEST(test1, "Loads a file and parses it"); */
  /* TEST(test2, "parse trace from string"); */
  TEST(test3, "single process simulation");
  /* TEST(test4, "single process, multiple simulations"); */
  /* TEST(test5, "multiple-process simulation"); */

  return 0;
}
