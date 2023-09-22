#include "mysh.h"

int main(){
    if(isatty(STDIN_FILENO) == 1) interactive();
    else batch();
    return 0;
}

char *readInput(){
    char buffer[BUFSIZE];
    int bytes;
    while((bytes = read(0,buffer,BUFSIZE)>0)){
        bytes = read(0, buffer, BUFSIZE);
    }
    return buffer;
}

char **parseInput(char *buffer){
    char **tokens = malloc(BUFSIZE * sizeof(char*));
    char *token;
    token = strtok(buffer, DELIMINATORS);
    
    for(int i = 0; token != NULL; i++){
        tokens[i] = token;
        token = strtok(NULL, DELIMINATORS);
    }
    return tokens;
}

int checkBuiltin(char **args){
    char *builtIns[] = {"cd","pwd"};
    int(*builtin_func[])(char **) = {&cdBuiltin, &pwdBuiltin};
    
    for(int i = 0; i < sizeof(builtIns)/sizeof(char *); i++){
    if(strcmp(args[0], builtIns[i]) == 0) return ((*builtin_func[i])(args)); 
    }
    return (execute(args));
}
int execute(char **args){
    pid_t pid = fork();
    int wstatus;
    if (pid == 0){
        if(execvp(args[0], args) == -1){
            perror("error");
        }
        return EXIT_FAILURE;
    }
    else if(pid<0){
        perror("error");
        return EXIT_FAILURE;
    }
    else{
        do{ 
        waitpid(pid, &wstatus, 0);
        } while(!WIFEXITED(wstatus));
    }
    return EXIT_SUCCESS;
}

void interactive(){
    char *line;
    char **args;
    int wstatus = 0;

    do{
        printf("mysh> ");
        line = readInput();
        args = parseInput(line); 
        wstatus = execute(args);

        if(wstatus >= 0){
            printf("!mysh> ");
            exit(wstatus);
        }
    }while(wstatus == -1);
}

void batch(){
    char *cmnds;
    char **args;
    int wstatus = -1;

    do{
        cmnds = readInput();
        args = parseInput(cmnds); 
        wstatus = execute(args);
        if (wstatus >= 0){
            printf("!mysh> ");
            exit(wstatus);
        }
    }while(wstatus == -1);
}