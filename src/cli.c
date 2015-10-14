#include "memman/cli.h"

int mm_cli_command_carrega(const char* arg, mm_simulator_t* sim) { 
  /* mm_simulator_t* new_simulator; */

  if (!arg) {
    fprintf(stderr, "`carrega <fname>` expected a filename.\n");
    return 1;
  }

  /* new_simulator = mm_simulator_parse_file(arg); */
  
  return 0; 
}

int mm_cli_command_show(const char* arg, mm_simulator_t* sim)
{
  mm_simulator_show(sim);
  return 0;
}

int mm_cli_command_espaco(const char* arg, mm_simulator_t* sim)
{
  char* end = NULL;
  unsigned alg = 0;
  const char* ERR_MSG = "`%s` is not a valid free mem management algorithm.\n"
                        "Valid algorithms are:\n"
                        "  1\tFirst Fit\n"
                        "  2\tNext Fit\n"
                        "  3\tQuick Fit\n";

  if (!arg) {
    fprintf(stderr, ERR_MSG, NULL);
    return 1;
  }

  alg = strtoul(arg, &end, 10);
  if (arg == end || mm_simulator_set_free_mem_alg(sim, alg)) {
    fprintf(stderr, ERR_MSG, arg);
    return 1;
  }

  return 0;
}

int mm_cli_command_substitui(const char* arg, mm_simulator_t* sim)
{
  char* end = NULL;
  unsigned alg = 0;
  const char* ERR_MSG = "`%s` is not a valid page replacement algorithm.\n"
                        "Valid algorithms are:\n"
                        "  1\tNot Recently Used Page\n"
                        "  2\tFirst-In, First-Out\n"
                        "  3\tSecond-Chance Page\n"
                        "  4\tLeast Recently Used Page\n";

  if (arg) {
    fprintf(stderr, ERR_MSG, NULL);
    return 1;
  }

  alg = strtoul(arg, &end, 10);
  if (arg == end || mm_simulator_set_page_subst_alg(sim, alg)) {
    fprintf(stderr, ERR_MSG, arg);

    return 1;
  }

  return 0;
}

int mm_cli_command_executa(const char* arg, mm_simulator_t* sim)
{
  LOGERR("executing");
  if (!(sim->virtual && sim->physical && sim->trace_fname)) {
    fprintf(stderr,
            "Trace not properly loaded.\n"
            "Type `ajuda` if you need help configuring the simulator.\n");
    return 1;
  }

  /* mm_simulator_simulate(sim); */

  return 0;
}

int mm_cli_command_sai(const char* arg, mm_simulator_t* sim)
{
  fprintf(stderr, "Bye bye!\n");
  exit(0);
  return 0;
}

int mm_cli_command_ajuda(const char* arg, mm_simulator_t* sim)
{
  fprintf(stderr, "%s", MM_CLI_HELP);
  return 0;
}

void mm_cli_run()
{
  unsigned i = 0;
  char* input = NULL;
  char** argv = NULL;
  mm_cli_command cmd = NULL;
  mm_simulator_t* simulator = mm_simulator_create();

  rl_bind_key('\t', rl_complete);
  fprintf(stderr, "%s", MM_CLI_WELCOME);

  while (1) {
    input = readline(MM_CLI_PROMPT);
    if (!input)
      continue;

    add_history(input);
    argv = history_tokenize(input);
    if (!argv) {
      free(input);
      continue;
    }

    cmd = mm_cli_search_command(argv[0]);
    if (cmd != NULL)
      cmd(argv[1], simulator);
    else
      fprintf(stderr, "Couldn't find command %s\n", argv[0]);

    i = 0;
    free(input);
    while (argv[i])
      free(argv[i++]);
    free(argv);
  }

  mm_simulator_destroy(simulator);
}

mm_cli_command mm_cli_search_command(const char* cmd)
{
  int bottom = 0;
  int top = MM_CLI_COMMANDS_SIZE - 1;
  int mid;
  int res;

  while (bottom <= top) {
    mid = (bottom + top) / 2;

    if (!(res = strcmp(MM_CLI_COMMANDS[mid].key, cmd)))
      return MM_CLI_COMMANDS[mid].command;
    else if (res > 0)
      top = mid - 1;
    else if (res < 0)
      bottom = mid + 1;
  }

  return NULL;
}
