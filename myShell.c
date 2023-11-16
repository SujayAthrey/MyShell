#include "myShell.h"

int main() {
    char commands[INPUT_BUFFER];
    cmd_history history = {.count = 0};

    while (1) { //set condition to '1' so the shell continously executes commands until exited
        //display the shell prompt at the start of each entry
        printf("myShell> ");

        //retrieve user commands using 'fgets'
        fgets(commands, INPUT_BUFFER, stdin);
        commands[strcspn(commands, "\n")] = '\0';  //remove the newline character represented by '\0'

        //add the 'commands' or command to our history for the history() function
        add_cmd_to_history(&history, commands);

        //tokenize the commands into readable tokens
        char *tokens[TOKEN_BUFFER];
        int tok_cnt = 0;

        char *token = strtok(commands, DELIMINATORS);
        while (token != NULL && tok_cnt < TOKEN_BUFFER) {
            tokens[tok_cnt] = token;
            tok_cnt++;
            token = strtok(NULL, DELIMINATORS);
        }
        tokens[tok_cnt] = NULL;

        if (tok_cnt > 0) {
            //if there is an existing token, or input, we can now check whether the input is equal to any of our built in commands, if they are, we execute the function associated with each command
            if (strcmp(tokens[0], "cd") == 0) {
                if (tok_cnt == 2) {
                    cd_cmd(tokens[1]);
                } else {
                    fprintf(stderr, "Usage: cd <directory>\n");
                }
            } else if (strcmp(tokens[0], "ls") == 0) {
                exec_cmd(tokens, &history);
            } else if (strcmp(tokens[0], "pwd") == 0) {
                exec_cmd(tokens, &history);
            } else if (strcmp(tokens[0], "mkdir") == 0) {
                //create a new directory
                if (tok_cnt == 2) {
                    if (mkdir(tokens[1], 0777) != 0) {
                        perror("mkdir");
                    }
                } else {
                    fprintf(stderr, "Usage: mkdir <directory>\n");
                }
            } else if (strcmp(tokens[0], "rmdir") == 0 || strcmp(tokens[0], "rm") == 0) {
                //remove the directory
                if (tok_cnt == 2) {
                    if (rmdir(tokens[1]) != 0) {
                        perror("rmdir");
                    }
                } else {
                    fprintf(stderr, "Usage: rmdir <directory>\n");
                }
            } else if (strcmp(tokens[0], "history") == 0) {
                //display the command history
                display_history(&history);
            } else if (strcmp(tokens[0], "help") == 0) {
                //display the help information, which is a print statement regarding the available other functions
                help_cmd();
            } else if (strcmp(tokens[0], "exit") == 0) {
                //exit shell
                exit_cmd();
            }
        }
    }
    return 0;
}
