#include "memman/simulator.h"

mm_simulator_t* mm_simulator_create()
{
  mm_simulator_t* simulator = malloc(sizeof(*simulator));
  PASSERT(simulator, MM_ERR_MALLOC);

  *simulator = zeroed_simulator_t;

  return simulator;
}

void mm_simulator_destroy(mm_simulator_t* simulator)
{
  mm_memory_destroy(simulator->physical);
  mm_memory_destroy(simulator->virtual);
  mm_mmu_destroy(simulator->mmu);
  mm_seglist_destroy(simulator->segments);

  FREE(simulator->processes);
  FREE(simulator);
}
void mm_simulator_show(mm_simulator_t* simulator)
{
  fprintf(stderr, MM_SIMULATOR_SHOW_TEMPLATE, simulator->trace_fname,
          simulator->physical != NULL ? simulator->physical->size : 0,
          simulator->virtual != NULL ? simulator->virtual->size : 0,
          simulator->free_mem_alg, simulator->page_subst_alg);
}

int mm_simulator_set_free_mem_alg(mm_simulator_t* simulator, unsigned alg)
{
  switch (alg) {
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    default:
      return 1;
  }

  return 0;
}

int mm_simulator_set_page_subst_alg(mm_simulator_t* simulator, unsigned alg)
{
  switch (alg) {
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    default:
      return 1;
  }

  return 0;
}

static char const* mm_strgets(const char* str, char* buf, size_t n)
{
  char* peek = strchr(str, '\n');

  if (peek) {
    ASSERT(peek - str + 1 < n, "Bounds checking");
    strncpy(buf, str, peek - str + 1);
  }

  return peek;
}

mm_simulator_t* mm_simulator_parse(const char* src)
{
  char buf[MM_MAX_INPUT];
  mm_process_t** processes = NULL;
  mm_simulator_t* simulator = mm_simulator_create();
  unsigned physical, virtual;
  unsigned lines = 0;
  char const* la = src;
  char* end = NULL, * curr = NULL;
  char ch;

  while ((ch = *la++))
    if (ch == '\n')
      lines++;

  processes = calloc(lines, sizeof(*processes));
  ASSERT(processes, MM_ERR_MALLOC);
  lines = 0;

  // first line
  la = mm_strgets(src, buf, MM_MAX_INPUT);

  curr = buf;
  physical = strtoul(curr, &end, 10);
  ASSERT(curr != end, MM_ERR_MALFORMED_TRACE);
  simulator->physical = mm_memory_create(physical, MM_MEM_PHYSICAL);
  curr = end;

  virtual = strtoul(curr, &end, 10);
  ASSERT(curr != end, MM_ERR_MALFORMED_TRACE);
  simulator->virtual = mm_memory_create(virtual, MM_MEM_VIRTUAL);

  // processes' lines
  while ((la = mm_strgets(la + 1, buf, MM_MAX_INPUT)))
    processes[lines++] = mm_process_parse(buf);
  simulator->processes = processes;
  simulator->process_count = lines;

  simulator->segments = mm_seglist_create(virtual, MM_ALG_FREE_FF);
  simulator->mmu = mm_mmu_create(virtual, physical, MM_PAGE_SIZE, NULL);

  return simulator;
}

mm_simulator_t* mm_simulator_parse_file(const char* fname)
{
  mm_simulator_t* simulator;
  FILE* file = fopen(fname, "rt");
  char* data;
  unsigned long length;

  ASSERT(file, "Couldn't load file `%s`", fname);

  fseek(file, 0, SEEK_END);
  length = ftell(file);
  PASSERT((data = calloc(length + 1, sizeof(*data))), MM_ERR_MALLOC);
  fseek(file, 0, SEEK_SET);
  fread(data, 1, length, file);
  data[length] = '\0';
  fclose(file);

  simulator = mm_simulator_parse(data);

  free(data);

  return simulator;
}

static void process_event_handler(siginfo_t* signal, void* initial_data)
{
  mm_simulator_t* sim = (mm_simulator_t*)initial_data;
  mm_process_t* proc = NULL;
  mm_segment_t* segment = NULL;
  mm_process_access_t* proc_access = NULL;

  if (signal->si_signo == SIG_PROCESS_NEW) {
    unsigned j = 0;
    proc = (mm_process_t*)signal->si_ptr;
    segment = mm_seglist_add_proc16(sim->segments, proc);

    for (; j < proc->access_count; j++) {
      proc->access[j].position += segment->start;
      mm_timer_schedule(SIG_PROCESS_ACCESS, proc->access[j].time,
                        &proc->access[j]);
    }
  }

  else if (signal->si_signo == SIG_PROCESS_ACCESS) {
    unsigned phys_pos;

    proc_access = (mm_process_access_t*)signal->si_ptr;
    phys_pos = mm_mmu_access(sim->mmu, proc_access->position);
  }

  else if (signal->si_signo == SIG_PROCESS_END) {
    proc = (mm_process_t*)signal->si_ptr;
    ASSERT((segment = mm_seglist_search_process(sim->segments, proc)),
           "Process must be in segments list");
    mm_seglist_free_process(sim->segments, segment);
  }

  else if (signal->si_signo == SIG_PROCESS_QUANTUM) {
    LOGERR("Process QUANTUM!");
  }

  else {
    fprintf(stderr, "Unexpected signal `%d`.\n", signal->si_signo);
    exit(EXIT_FAILURE);
  }
}

void mm_simulator_simulate(mm_simulator_t* simulator)
{
  unsigned i = 0;
  unsigned events_count = 0;

  mm_memory_init_file(simulator->virtual);
  mm_memory_init_file(simulator->physical);

  mm_timer_init();
  for (; i < simulator->process_count; i++) {
    mm_timer_schedule(SIG_PROCESS_NEW, simulator->processes[i]->t0,
                      simulator->processes[i]);
    mm_timer_schedule(SIG_PROCESS_END, simulator->processes[i]->tf,
                      simulator->processes[i]);
    // acesses + (new + end)
    events_count += simulator->processes[i]->access_count + 2;
  }

  // TODO depending on the page-subst algorithm,
  //      set the quantum timer as well
  mm_timer_wait(events_count, (void*)simulator, process_event_handler);
}
