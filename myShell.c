#include "myShell.h"

int (*builtin_func[])(char **) = {&cdShell, &helpShell, &exitShell};

char *read(void){
    int bufferSize = BUFSIZE;
    char *buffer = malloc(sizeof(char) * bufferSize);
    int idx = 0, newChar;

    //check if buffer correctly allocates memory
    if(!buffer){
        perror("Allocation error: buffer could not be allocated\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        //Get and read a new character from the command line
        newChar = getchar();  
        //Checks if we have reached the end of file ended, if so, we can exit as everything has been read
        if(newChar == EOF){  
            exit(EXIT_SUCCESS);
        }else if(newChar == '\n'){
            //if we have reached end of line, add a null character at that index
            buffer[idx] = '\0';
            return buffer;
        }
        //add the new character read to the buffer
        else{
            buffer[idx] = newChar;
        }
        //increment buffer index
        idx++;

        //Check if buffer memory has been exceded (more chars to read than buffer can hold)
        //if true, reallocate the buffer to an additional 1024 size
        if(idx >= bufferSize){
            bufferSize += BUFSIZE;
            buffer = realloc(buffer, bufferSize);
            //check once again if buffer correctly allocates memory
            if(!buffer){ 
                perror("Allocation error: buffer could not be allocated\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **parse(char *args){
    int bufferSize = PARSEBUFSIZE, idx = 0;
    char **arguments = malloc(bufferSize * sizeof(char*)), *arg, **tempargs;

    //check if buffer correctly allocates memory
    if(!arguments){
        perror("Allocation error: buffer could not be allocated\n");
        exit(EXIT_FAILURE);
    }

    arg = strtok(args, DELIMINATORS);
    while(arg != NULL){
        //add new argument or "token" to the buffer whilst there are still arguments to be parsed
        arguments[idx] = arg;
        idx++;
        ///Check if buffer memory has been exceded (more chars to read than buffer can hold)
        //if true, reallocate the buffer to an additional 256 size
        if(idx >= bufferSize){
            bufferSize += PARSEBUFSIZE;
            tempargs = arguments;
            arguments = realloc(arguments, bufferSize * sizeof(char*));
            //check once again if buffer correctly allocates memory
            if(!arguments){
                free(tempargs);
                perror("Allocation error: buffer could not be allocated\n");
                exit(EXIT_FAILURE);
            }
        }
        arg = strtok(NULL, DELIMINATORS);
    }
    arguments[idx] = NULL;
    return arguments;
}

int forkIt(char **args){
    pid_t pid; //process ID's (pid of child process returns 0, pid of parent process returns greater than zero)
    int status;

    pid = fork(); 
    //runs fork function, returning two concurrent processes (child and parent)
    if(pid == 0){
        //we are in child process
        //runs execvp function to run a command that was read and parsed
        //if execvp returns value, there was an error in running command
        if(execvp(args[0], args) == -1){
            perror("Could not run command\n");
        }
        exit(EXIT_FAILURE); //exit shell so program continues running
    }
    else if(pid > 0){
    //we are in parent process
        do{
            //child process should execute succesfully, parent process will folow next
            //wait for child process to either be executed and exited or quit
            waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    else if(pid < 0){
        //could not fork processes (forking error)
        perror("Error in forking\n");
    }
  return 1;
}

int executeShell(char **args){
    //continues forward if no arguments passed
    if(args[0] == NULL) return 1;
    
    for(int i = 0; i < 3; i++){
        //checks to see if any of the built in commands were typed in as an argument by the user
        if(strcmp(args[0], "cd") == 0 || strcmp(args[0], "help") == 0 || strcmp(args[0], "exit") == 0){
            //executes that specific command
            return (*builtin_func[i])(args);
        }
    }
    return forkIt(args);
}

int cdShell(char **args){
    //check if there is a "cd" argument passed in at all
    if(args[1] == NULL){
        perror("Error: no directory passed in to be changed\n");
    }else{
    //chdir() should change the working directory to the corrent one and return 0
        if(chdir(args[1]) != 0){
            perror("Error: working directory could not be changed\n");
        }
    }
    return 1;
}

int helpShell(char **args){
    printf("Welcome to Sujay Athrey's shell!\n To operate this shell, please correctly type your commands and file names");
    printf("This shell is provided with the 'cd' and 'exit' built in functions\n");
    printf("This shell is designed to be simple in nature, it does not include several built in functions such as 'ls', 'pwd', and 'echo'.\n");
    return 1;
}

int exitShell(char **args){
  return EXIT_SUCCESS;
}

int runShell(void){
    char *cmndLine;
    char **arguments;
    int status;
    printf("Welcome to Sujay's Shell!!");
    do{
        printf("$ ");
        cmndLine = read();
        arguments = parse(cmndLine);
        status = executeShell(arguments);

        free(cmndLine);
        free(arguments);
    } while(status);
}

int main(int argc, char **argv){
    runShell();
    return EXIT_SUCCESS;
}
