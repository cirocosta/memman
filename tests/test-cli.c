#include "memman/cli.h"

void test1()
{
  unsigned count = 0;
  mm_cli_command cmd = mm_cli_search_command("ajuda");

  ASSERT(cmd != NULL, "");

  for (; count < MM_CLI_COMMANDS_SIZE; count++) {
    ASSERT(mm_cli_search_command(MM_CLI_COMMANDS[count].key) != NULL,
           "Command %s not found", MM_CLI_COMMANDS[count].key);
  }

  ASSERT(mm_cli_search_command("hue") == NULL, "");
}

int main(int argc, char *argv[])
{
  TEST(test1, "Commands are handled properly");

  return 0;
}
