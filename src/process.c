#include "memman/process.h"

mm_process_t* mm_process_create()
{
  mm_process_t* process = malloc(sizeof(*process));
  PASSERT(process, MM_ERR_MALLOC);

  *process = zeroed_process_t;

  return process;
}

void mm_process_destroy(mm_process_t* process) { FREE(process); }

mm_process_t* mm_process_parse(const char* src)
{
  char* curr = NULL;
  char* end = NULL;
  unsigned access_count = 0;
  mm_process_t* process = mm_process_create();

  // t0 : ul
  process->t0 = strtoul(src, &curr, 10);
  curr++;

  // pname: string
  end = strchr(curr, ' ');

  ASSERT(end, MM_ERR_MALFORMED_TRACE);
  strncpy(process->pname, curr, end - curr);
  process->pname[end - curr] = '\0';
  curr = end;

  // tf: ul
  process->tf = strtoul(curr, &end, 10);
  ASSERT(curr != end, MM_ERR_MALFORMED_TRACE);
  curr = end;

  // b: ul
  process->b = strtoul(curr, &end, 10);
  ASSERT(curr != end, MM_ERR_MALFORMED_TRACE);
  curr = end;

  // +(pn tn)
  while (*curr != '\0' && *curr != '\n') {
    process->access[access_count].proc = process;

    // pn: ul
    process->access[access_count].position = strtoul(curr, &end, 10);
    ASSERT(curr != end, MM_ERR_MALFORMED_TRACE);
    curr = end;

    // tn: ul
    process->access[access_count].time = strtoul(curr, &end, 10);
    ASSERT(curr != end, MM_ERR_MALFORMED_TRACE);
    curr = end;

    access_count++;
  }

  process->access_count = access_count;

  return process;
}
