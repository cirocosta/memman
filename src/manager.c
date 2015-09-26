#include "memman/manager.h"

mm_manager_t* mm_manager_create()
{
  mm_manager_t* manager = malloc(sizeof(*manager));
  PASSERT(manager, MM_ERR_MALLOC);

  *manager = zeroed_manager_t;

  return manager;
}

void mm_manager_destroy(mm_manager_t* manager)
{
  while (manager->process_count-- > 0)
    mm_process_destroy(manager->processes[manager->process_count]);
  FREE(manager->processes);
  FREE(manager);
}

// TODO transform this into a simple wrapper
//      around a parser that takes a string.
mm_manager_t* mm_manager_parse_file(const char* fname)
{
  char buf[MM_MAX_INPUT];
  FILE* fp = NULL;
  unsigned lines = 0;
  char* curr = NULL, * end = NULL;
  int ch;
  mm_process_t** processes = NULL;
  mm_manager_t* manager = mm_manager_create();

  PASSERT((fp = fopen(fname, "r")), "Error while opening file %s", fname);

  while ((ch = fgetc(fp)) != EOF) {
    if (ch == '\n')
      lines++;
  }

  fseek(fp, 0, SEEK_SET);
  processes = calloc(lines, sizeof(*processes));
  ASSERT(processes, "mem allocation error");
  lines = 0;

  // first line
  fgets(buf, MM_MAX_INPUT, fp);

  curr = buf;
  manager->physical = strtoul(curr, &end, 10);
  ASSERT(curr != end, MM_ERR_MALFORMED_TRACE);
  curr = end;

  manager->virtual = strtoul(curr, &end, 10);
  ASSERT(curr != end, MM_ERR_MALFORMED_TRACE);
  curr = end;

  // processes' lines
  while (fgets(buf, MM_MAX_INPUT, fp))
    processes[lines++] = mm_process_parse(buf);
  manager->processes = processes;

  ASSERT(~fclose(fp), "Error while closing file %s: %s", fname,
         strerror(errno));
  manager->process_count = lines;

  return manager;
}
