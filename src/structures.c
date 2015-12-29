#include "structures.h"

/**
 * Resets a command's attributes
 */
void command_reset(COMMAND* c)
{
    c->argc = 0;
    c->detached = WAIT;
    c->err_to_out = 0;
    c->redir_in.active = 0;
    c->redir_out.active = 0;
    c->redir_err.active = 0;
    c->read_pipe.active = 0;
    c->write_pipe.active = 0;
}

/**
 * Dynamic realloc of commands
 */
void command_alloc(COMMANDLIST *commands, int com_index)
{
    commands->list = (COMMAND*) realloc(commands->list,	(commands->size+1)*sizeof(COMMAND));
    commands->size++;
    COMMAND* next_com = &commands->list[com_index];
    command_reset(next_com);
}

/**
 * Dynamic (re)alloc of pipes
 */
void pipe_alloc(PIPES *pipes)
{// New pipe
    if(pipes->desc == NULL) pipes->desc = (int**) malloc(1*sizeof(int*));
    else pipes->desc = (int**) realloc(pipes->desc, (pipes->size+1)*sizeof(int*));
    pipes->desc[pipes->size] = malloc(2*sizeof(int));
    // Create pipe
    pipe(pipes->desc[pipes->size]);
    pipes->size++;
}