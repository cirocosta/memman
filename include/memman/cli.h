#ifndef MEMMAN__CLI_H
#define MEMMAN__CLI_H

#include "memman/common.h"
#include "memman/simulator.h"

#include <readline/readline.h>
#include <readline/history.h>

typedef int (*mm_cli_command)(const char* argument, mm_simulator_t* simulator);

typedef struct mm_cli_command_t {
  char* key;
  mm_cli_command command;
} mm_cli_command_t;

static const char* MM_CLI_PROMPT = "[mm] ";

#define MM_CLI_COMMANDS_SIZE 7

int mm_cli_command_carrega(const char* arg, mm_simulator_t* sim);
int mm_cli_command_espaco(const char* arg, mm_simulator_t* sim);
int mm_cli_command_substitui(const char* arg, mm_simulator_t* sim);
int mm_cli_command_executa(const char* arg, mm_simulator_t* sim);
int mm_cli_command_sai(const char* arg, mm_simulator_t* sim);
int mm_cli_command_ajuda(const char* arg, mm_simulator_t* sim);
int mm_cli_command_show(const char* arg, mm_simulator_t* sim);

const static char* MM_CLI_HELP =
    "Commands:\n"
    "  ajuda            Shows this help text\n"
    "  carrega <fname>  loads the given <filename> into the simulator\n"
    "  espaco <num>     specifies the freespace manager algorithm\n"
    "  substitui <num>  specifies the page substitution algorithm\n"
    "  executa <num>    executes the simulator printing in a `num` interval\n"
    "  show             displays the current simulator configuration\n"
    "  sai              exits the simulator\n"
    "\n"
    "Free Space Manager Algorithms:\n"
    "  1\tFirst Fit\n"
    "  2\tNext Fit\n"
    "  3\tQuick Fit\n"
    "\n"
    "Page Substitution Algorithms:\n"
    "\n"
    "  1\tNot Recently Used Page\n"
    "  2\tFirst-In, First-Out\n"
    "  3\tSecond-Chance Page\n"
    "  4\tLeast Recently Used Page\n";

const static char* MM_CLI_WELCOME = "\n"
                                    "Welcome to MEMMAN\n"
                                    "If you need help, type `ajuda`.\n\n";

const static mm_cli_command_t MM_CLI_COMMANDS[] = {
  { "ajuda", &mm_cli_command_ajuda },
  { "carrega", &mm_cli_command_carrega },
  { "espaco", &mm_cli_command_espaco },
  { "executa", &mm_cli_command_executa },
  { "sai", &mm_cli_command_sai },
  { "show", &mm_cli_command_show },
  { "substitui", &mm_cli_command_substitui },
};

void mm_cli_run();
mm_cli_command mm_cli_search_command(const char* cmd);

#endif
