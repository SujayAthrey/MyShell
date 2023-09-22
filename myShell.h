#ifndef MYSH_H
#define MYSH_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define DELIMINATORS " \t\r\n\v\\|\\>\\<\\*"
#define BUFSIZE 512

void interactive();
void batch();
char *readInput();
char **parseInput(char *cmnds);
int checkBuiltin(char **args);
int execute(char **args);
int cdBuiltin(char **args);
int pwdBuiltin(char **args);

#endif