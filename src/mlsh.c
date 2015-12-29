#include "mlsh.h"

/**
 * Displays help and stuff
 */
void init()
{
	printf(PREFIX " Hello World!\n");
	printf(PREFIX " Version 2.1.5\n");
	printf(PREFIX " Help\n");
	printf("# Buildin functions \n");
	printf("# cd DIR : change working directory\n");
	printf("# help : display this help section\n");
	printf("# exit : quit MyLittleShell\n");
	printf("# Features\n");
	printf("# < (stdin) > and >> (stdout) 2>  2>&1 and 2>> (stderr) redirection\n");
	printf("# | pipe (multiple pipes are supported)\n");
	printf("# & detach process (not while piping)\n");
	printf("# && execute next command if success\n");
	printf("# || execute next command if failure\n");
	printf("# && and || work with cd DIR \n");
	printf("# Examples \n");
	printf("# dia & \n");
	printf("# ls > a \n");	
	printf("# ls Omainen 2>&1 > a \n");
	printf("# ./make && ./run\n");
	printf("# ls  neurinome/ || ls \n");
	printf("# grep \\.c$ < a \n");
	printf("# cd .. \n");
	printf("# cd .. && ls \n");
	printf("# ls | grep .c | grep \\.c$ \n");
	printf("# You can compile and run MyLittleShell in MyLittleShell \n");
	printf("# (in MyLittleShell root dir) \n");
	printf("# ./configure \n");
	printf("# ./make \n");
	printf("# ./run \n");
	printf("# Have fun using MyLittleShell!\n");
}

/**
 * Handles buildin commands
 * cd, exit, help
 *  No pipe is handled as in BASH shell (makes no sense to pipe before / after
 *  a buildin command)
 */
int buildin_commands(char** ptr_buffer)
{
	int b = 1;
	char* buffer = *ptr_buffer;
	if(strncmp(buffer, "exit", 4)==0)
	{
		printf("exit\n");
		return 0;
	}
	else if(strncmp(buffer, "cd", 2)==0)
	{
		buffer = strtok(NULL," ");
		b = chdir(buffer);
		if (b<0) printf("cd : No such directory\n");
	}
	else if(strncmp(buffer, "help", 4)==0)
	{
		init();
		b=0;
	}
	// Emptying buffer (no piping is supported)
	if(b<1)
	{
		while(buffer != NULL)
		{
			if(strncmp(buffer, "&&",2) == 0 && b == 0) break;
			if(strncmp(buffer, "||",2) == 0 && b == -1) break;
			else buffer = strtok(NULL," ");

		}
		buffer = strtok(NULL," ");
	}

	*ptr_buffer = buffer;
	return 1;
}



/**
 * Analyses a command extracted from scanf.
 * This can be recursive if commands are piped: in this case,
 * pipe and next command are dynamically allocated. (exit when buffer is empty)
 * Multiple pipes are handled.
 */
void analyse(PIPES* pipes, COMMANDLIST* commands, char* first_token, int command_index)
{
	//printf("Analysing\n");
	char* buffer = first_token;
	int com_index = command_index;
	COMMAND* com = &commands->list[com_index];

	while(buffer != NULL)
	{
		//printf("%s\n",buffer);

		if(strncmp(buffer, "&&",2) == 0)
		{
			com->detached = WAIT_SUCCESS;
			char* token = strtok(NULL," ");
			int next_index = com_index+1;
			command_alloc(commands, next_index);
			// Analyze Command 2 args
			analyse(pipes, commands, token, next_index);
		}
		else if(strncmp(buffer, "||",2) == 0)
		{
			com->detached = WAIT_FAIL;
			char* token = strtok(NULL," ");
			int next_index = com_index+1;
			command_alloc(commands, next_index);
			// Analyze Command 2 args
			analyse(pipes, commands, token, next_index);
		}
		else if(strncmp(buffer, ">>",2) == 0)
		{
			// STDOUT Redirection
			com->redir_out.path = strtok(NULL," ");
			com->redir_out.active = 1;
			com->redir_out.mode = APPEND;
		}
		else if(strncmp(buffer, ">",1) == 0)
		{
			// STDOUT Redirection
			com->redir_out.path = strtok(NULL," ");
			com->redir_out.active = 1;
			com->redir_out.mode = WRITE;
		}
		else if(strncmp(buffer, "2>&1",4) == 0)
		{
			// STDERR Redirection
			com->err_to_out = 1;
		}
		else if(strncmp(buffer, "2>>",3) == 0)
		{
			// STDERR Redirection
			com->redir_err.path = strtok(NULL," ");
			com->redir_err.active = 1;
			com->redir_err.mode = APPEND;
		}
		else if(strncmp(buffer, "2>",1) == 0)
		{
			// STDERR Redirection
			com->redir_err.path = strtok(NULL," ");
			com->redir_err.active = 1;
			com->redir_err.mode = WRITE;
		}
		else if(strncmp(buffer, "<",1) == 0)
		{
			// STDIN Redirection
			com->redir_in.path = strtok(NULL," ");
			com->redir_in.active = 1;
			com->redir_in.mode = READ;
		}else if(strncmp(buffer, "|",1) == 0)
		{
			//	PIPE Redirection
			// This calls for some realloc and recursive stuff!
			char* token = strtok(NULL," ");

			// New pipe
			pipe_alloc(pipes);
			int pipe_index = pipes->size - 1;
			// Command 1 pipe
			com->write_pipe.active = 1;
			com->write_pipe.desc = pipes->desc[pipe_index];
			int next_index = com_index+1;
			// New command
			command_alloc(commands, next_index);
			// realloc might have moved ptr of com
			com = &commands->list[com_index];

			COMMAND* next_com = &commands->list[next_index];
			// Command 2 pipe
			next_com->read_pipe.active = 1;
			next_com->read_pipe.desc = pipes->desc[pipe_index];

			// Analyze Command 2 args
			analyse(pipes, commands, token, next_index);

		}
		else
		{
			// Simply add buffer to the argv
			com->argv[com->argc] = buffer;
			// increase argc count
			com->argc++;
		}
		buffer = strtok(NULL," ");
	}


	if(strncmp(com->argv[com->argc-1], "&", 1) == 0)
	{
		if( com->write_pipe.active == 1)
		{
			printf(PREFIX FONT_RED " Cannot detach process AND pipe its output" COLOR_RESET "\n");
		}else
		{
			// will detached process from MLSH
			// Changes arg "&" by NULL
			com->argv[com->argc-1] = NULL;
			com->detached = DETACHED;
		}

	}
	else if(com->detached < WAIT) com->detached = WAIT;
	com->argv[com->argc] = NULL;

}

/**
 * Executes the Command c in another process
 */
int execute(COMMAND* c)
{
//	printf("Fork\n");
	int status;
	int pid = fork();
	if(pid != 0)
	{
		// pipes are handled only if command is not detached
		if(c->detached > DETACHED)
		{
			waitpid(pid, &status, 0);
			// Don't forget to close pipes once used!
			if(c->read_pipe.active == 1) close(c->read_pipe.desc[PIPE_READ]);
			if(c->write_pipe.active == 1) close(c->write_pipe.desc[PIPE_WRITE]);
			if(c->detached > DETACHED) c->child_code = WEXITSTATUS(status);

		}

	}
	else
	{
		if(c->redir_out.active == 1)
		{
			// STDOUT Redirection
			freopen(c->redir_out.path, c->redir_out.mode, stdout);
		}
		if(c->redir_in.active == 1)
		{
			// STDIN Redirection
			freopen(c->redir_in.path, c->redir_in.mode, stdin);
		}
		if(c->redir_err.active == 1)
		{
			// STDERR Redirection
			freopen(c->redir_err.path, c->redir_err.mode, stderr);
		}if(c->err_to_out == 1)
		{
			// STDERR Redirection
			dup2(STDOUT_FILENO, STDERR_FILENO);
		}

		if(c->read_pipe.active == 1)
		{
			// Read from PIPE
			PIPE* p = &c->read_pipe;
			close(p->desc[PIPE_WRITE]);
			dup2(p->desc[PIPE_READ], STDIN_FILENO);
		}
		if(c->write_pipe.active == 1)
		{
			// Write to PIPE
			PIPE* p = &c->write_pipe;
			close(p->desc[PIPE_READ]);
			dup2(p->desc[PIPE_WRITE], STDOUT_FILENO);
		}

		// Execute command, Yay
		if(execvp(c->argv[0],c->argv) == -1)
		{
			// It fails...
			printf(PREFIX FONT_RED " Program not found:" COLOR_RESET " %s\n", c->argv[0]);
			return -1;
		}
	}

	return 0;

}

/**
 * Main Function
 */
int main(int argc, char **argv)
{
	int i,j;
	char commande[100];
	char *buffer;
	// usefull structs
	PIPES pipes;
	COMMANDLIST commands;

	// At least one command
	commands.size = 1;
	commands.list = (COMMAND*) malloc(commands.size*sizeof(COMMAND));

	// Pipes!
	pipes.size = 0;
	pipes.desc = NULL;

 	init();
	// Main loop (nice while(1))
	while(1)
	{
		// Main prompt
		printf(PREFIX FONT_MAGENTA "<3 " COLOR_RESET);
		scanf("%[^\n]%*c",commande);

		// Split commande into tokens
		buffer = strtok(commande," ");
		// Handle buildin commands
		if(buildin_commands(&buffer) == 0) break;
		// if buffer is null (after buildin commands handle), return to the prompt

		if(buffer == NULL) continue;

		commands.size = 1;
		pipes.size = 0;
		command_reset(&commands.list[0]);
		analyse(&pipes, &commands, buffer, 0);
		// execute each command in the list
		for(i = 0;i< commands.size;i++)
		{
			int b = 0;
			if(i>0)
			{
				COMMAND* last_com = &commands.list[i-1];
				if(last_com->detached == WAIT || last_com->detached == DETACHED) b = 1;
				else if(last_com->detached == WAIT_FAIL && last_com->child_code > 0) b = 1;
				else if(last_com->detached == WAIT_SUCCESS && last_com->child_code == 0) b = 1;
			}
			else b = 1;
			if(b==1) execute(&commands.list[i]);
		}

	}

	// free malloc'd stuff
	free((void*)commands.list);
	for(j = 0; j< pipes.size;j++) free(pipes.desc[j]);
	free(pipes.desc);
	printf(PREFIX " Goodbye!\n");
	printf(PREFIX " You have lost (The Game)\n");
	return 0;
}
