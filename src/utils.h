/**
 * MyLittleShell
 * Version 2.1.5
 */

#ifndef MLSH_UTILS_H
#define MLSH_UTILS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <malloc.h>


// Console colors
// Pretty print!
#define FONT_RED     "\x1b[31m"
#define COLOR_RED     "\x1b[41m"
#define FONT_BLACK    "\x1b[30m"
#define FONT_WHITE    "\x1b[37m"
#define COLOR_BLUE    "\x1b[44m"
#define COLOR_MAGENTA "\x1b[45m"
#define FONT_MAGENTA  "\x1b[35m"
#define COLOR_WHITE   "\x1b[47m"
#define COLOR_RESET   "\x1b[0m"

// Macro for [MyLittleShell]
#define PREFIX "[" FONT_WHITE COLOR_BLUE "My" FONT_BLACK COLOR_WHITE "Little" FONT_WHITE COLOR_RED "Shell" COLOR_RESET "]"


#define DETACHED 0
#define WAIT 1
#define WAIT_FAIL 2
#define WAIT_SUCCESS 3

#define PIPE_READ 0
#define PIPE_WRITE 1
#define APPEND "a"
#define WRITE "w"
#define READ "r"

#endif //MLSH_UTILS_H
