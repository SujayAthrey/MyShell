#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define DELIMINATORS " \t\r\n\a"
#define BUFSIZE 1024
#define PARSEBUFSIZE 256

char *read(void);
char **parse(char *args);
int forkIt(char **args);
int executeShell(char **args);
int runShell(void);

//built ins
int cdShell(char **args);
int helpShell(char **args);
int exitShell(char **args);


#endif