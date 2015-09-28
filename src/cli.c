#include "memman/cli.h"

int mm_cli_command_carrega(const char* arg)
{
  LOGERR("executing carrega");
  return 0;
}

int mm_cli_command_espaco(const char* arg)
{
  LOGERR("executing espaco");
  return 0;
}

int mm_cli_command_substitui(const char* arg)
{
  LOGERR("executing substitui");

  return 0;
}

int mm_cli_command_executa(const char* arg)
{
  LOGERR("executing intervalo");
  return 0;
}

int mm_cli_command_sai(const char* arg)
{
  fprintf(stderr, "Bye bye!\n");
  exit(0);
  return 0;
}

int mm_cli_command_ajuda(const char* arg)
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

  rl_bind_key('\t', rl_complete);
  fprintf(stderr, "%s", MM_CLI_WELCOME);

  while (1) {
    input = readline(MM_CLI_PROMPT);
    if (!input)
      continue;

    add_history(input);
    argv = history_tokenize(input);
    if (!argv)
      continue;

    cmd = mm_cli_search_command(argv[0]);
    if (cmd != NULL) {
      cmd(argv[1]);
    } else {
      fprintf(stderr, "Couldn't find command %s\n", argv[0]);
    }

    i = 0;
    FREE(input);
    while (argv[i]) {
      FREE(argv[i++]);
    }
  }
}

mm_cli_command mm_cli_search_command(const char* cmd)
{
  int bottom = 0;
  int top = MM_CLI_COMMANDS_SIZE - 1;
  int mid;
  int res;

  while (bottom <= top) {
    mid = (bottom + top) / 2;
    if (!(res = strcmp(MM_CLI_COMMANDS[mid].key, cmd))) {
      return MM_CLI_COMMANDS[mid].command;
    } else if (res > 0) {
      top = mid - 1;
    } else if (res < 0) {
      bottom = mid + 1;
    }
  }

  return NULL;
}
