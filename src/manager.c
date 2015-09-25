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
  FREE(manager);
}

mm_manager_t* mm_manager_read_file(const char* fname, unsigned* count)
{
  /* char buf[MM_MAX_INPUT]; */
  /* FILE* fp = NULL; */
  /* unsigned lines = 0; */
  /* int ch; */
  mm_manager_t* manager = mm_manager_create();

  /* PASSERT((fp = fopen(fname, "r")), "Error while opening file %s", fname); */

  /* while ((ch = fgetc(fp)) != EOF) { */
  /*   if (ch == '\n') */
  /*     lines++; */
  /* } */

  /* fseek(fp, 0, SEEK_SET); */
  /* traces = malloc(sizeof(*traces) * lines); */
  /* ASSERT(traces, "mem allocation error"); */
  /* lines = 0; */

  /* // perform a fgets for the first line and parse it */
  /* fgets .. . */ 

  /* // then parse the using process' parser */

  /* while (fgets(buf, MAX_INPUT, fp)) */
  /*   traces[lines++] = sm_parse_trace(buf); */

  /* ASSERT(~fclose(fp), "Error while closing file %s: %s", fname, */
  /*        strerror(errno)); */
  /* *entries = lines; */

  return manager;
}

