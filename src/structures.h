/**
 * MyLittleShell
 * Version 2.1.5
 */

#ifndef MLSH_STRUCTURES_H
#define MLSH_STRUCTURES_H

#include "utils.h"

/**
 * Defines a redirection stream
 */
typedef struct
{
    int active;
    char* path;
    char* mode;

} REDIRECTION;

/**
 * Defines a pipe
 */
typedef struct
{
    int* desc;
    int active;
} PIPE;

/**
 * Defines the list of pipes used in the program
 */
typedef struct
{
    int** desc;
    int size;
} PIPES;

/**
 * Defines a command struct:
 * STDIN / STDOUT / PIPE redirection
 * Arguments
 * Detached or not
 */
typedef struct
{
    REDIRECTION redir_out;
    REDIRECTION redir_err;
    REDIRECTION redir_in;
    int err_to_out;
    PIPE read_pipe;
    PIPE write_pipe;
    char *argv[100];
    int argc;
    int detached;

    int child_code;
} COMMAND;

/**
 * Defines the list of commands to execute
 */
typedef struct
{
    COMMAND* list;
    int size;
} COMMANDLIST;


void command_reset(COMMAND* c);
void command_alloc(COMMANDLIST *commands, int com_index);

void pipe_alloc(PIPES *pipes);

#endif //MLSH_STRUCTURES_H
