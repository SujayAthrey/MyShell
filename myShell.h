#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#define INPUT_BUFFER 1024
#define TOKEN_BUFFER 100
#define HISTORY_BUFFER 20
#define DELIMINATORS " \t\r\n\a"

// Structure to store command history
typedef struct {
    char entries[HISTORY_BUFFER][INPUT_BUFFER];
    int count;
} cmd_history;

char *readInput(void);
char **parseInput(char *input);
void exec_cmd(char *tokens[], cmd_history *history);
void add_cmd_to_history(cmd_history *history, const char *command);
void display_history(const cmd_history *history);

//built ins
void cd_cmd(char *path);
void help_cmd(void);
void exit_cmd(void);

#endif
