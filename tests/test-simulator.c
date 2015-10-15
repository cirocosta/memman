#include "memman/simulator.h"

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

  mm_simulator_destroy(simulator);
}

void test2()
{
  const char* src = "100 100\n"
                    "0 process0 2 10 1 1 2 2\n"
                    "0 process1 2 10 1 1 2 2\n"
                    "0 process2 2 10 1 1 2 2\n"
                    "0 process3 2 10 1 1 2 2\n";

  mm_simulator_t* simulator = mm_simulator_parse(src);

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

  mm_simulator_destroy(simulator);
}

void test3()
{
  //                phys virt
  const char* src = "32 64\n"
                    // t0 pname tf b pn tn
                    "1 process2 3 16 0 1\n";
  mm_simulator_t* simulator = mm_simulator_parse(src);

  mm_simulator_simulate(simulator);

  mm_simulator_destroy(simulator);
}

int main(int argc, char* argv[])
{
  TEST(test1, "Loads a file and parses it");
  TEST(test2, "parse trace from string");
  TEST(test3, "single process simulation");

  return 0;
}
