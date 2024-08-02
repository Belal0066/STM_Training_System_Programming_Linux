#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>


void pwd_command();
void echo_command(char **argv);
void cp_command(char **argv);
int mv_command(char **argv);

void type_command(const char *command);
int is_builtin(const char *command);
int is_executable(const char *command);

void env_command();
void cd_command(char **argv);

#endif // COMMANDS_H
