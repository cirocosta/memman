#ifndef MEMMAN__SIMULATOR_H
#define MEMMAN__SIMULATOR_H

#include "memman/common.h"
#include "memman/process.h"
#include "memman/memory.h"
#include "memman/timer.h"
#include "memman/seglist.h"
#include "memman/mmu.h"

#include <math.h>

// FIXME timer_create() allocate resources.
//       we must take care of this later.
//       ==> timer_delete(tid)

typedef struct mm_simulator_t {
  const char* trace_fname;
  mm_memory_t* physical;
  mm_memory_t* virtual;

  mm_algorithms_e free_mem_alg;   // for the seglist
  mm_algorithms_e page_subst_alg; // for the mmu

  mm_process_t** processes; // all processes parsed
  unsigned process_count;   // # of processes parsed

  mm_seglist_t* segments; 
  mm_mmu_t* mmu;
} mm_simulator_t;

static const mm_simulator_t zeroed_simulator_t = { 0 };

mm_simulator_t* mm_simulator_create();
void mm_simulator_destroy(mm_simulator_t*);

int mm_simulator_set_free_mem_alg(mm_simulator_t* simulator, unsigned alg);
int mm_simulator_set_page_subst_alg(mm_simulator_t* simulator, unsigned alg);
void mm_simulator_show(mm_simulator_t* simulator);
void mm_simulator_simulate(mm_simulator_t* simulator);

mm_simulator_t* mm_simulator_parse_file(const char* fname);
mm_simulator_t* mm_simulator_parse(const char* src);
void mm_simulator_create_memory(mm_simulator_t* simulator);

static const char* MM_SIMULATOR_SHOW_TEMPLATE = "Simulator status:\n"
                                                "├─trace file ----- %d\n"
                                                "├─physical ------- %d\n"
                                                "├─virtual -------- %d\n"
                                                "├─free space alg - %d\n"
                                                "└─page subst alg - %d\n";
#endif
