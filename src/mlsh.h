/**
 * MyLittleShell
 * Version 2.1.5
 */

#ifndef MLSH_MLSH_H
#define MLSH_MLSH_H

#include "utils.h"
#include "structures.h"

void init();
int buildin_commands(char** buffer);
void analyse(PIPES* pipes, COMMANDLIST* commands, char* first_token, int command_index);
int execute(COMMAND* c);

#endif //MLSH_MLSH_H