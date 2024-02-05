#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

// Define MAXSIZE
int MAXSIZE = 20;
int DEBUG = 0;

// Define parse_command()
char **parse_command(const char *command) {
    // Allocate memory for arguments
    char **args = malloc(sizeof(char*) * MAXSIZE);
    if (!args) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    char *token = strtok((char *)command, " ");
    while (token != NULL) {
        // Print token if DEBUG is enabled
        if (DEBUG == 1) { 
            printf("%s\n", token); 
        }
        // Store token in arguments array
        args[index] = strdup(token);
        if (!args[index]) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        index++;
        // Check for too many arguments
        if (index >= MAXSIZE) {
            fprintf(stderr, "Too many arguments\n");
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, " ");
    }
    // Terminate arguments array with NULL
    args[index] = NULL;
    return args;
}

//Define execute_command()
void execute_command(char** args){
    char* __path;

    if(args[0][0] == '/'){
        //__path is absolute
        __path = args[0]; //use the first argument as the path
        printf("0\n");
    }
    else{
        //__path is relative
        __path = malloc(strlen("/bin/") + strlen(args[0]) + 1); //allocate memory for the path
        strcpy(__path, "/bin/"); //copy "/bin/" to the path
        strcat(__path, args[0]); //append the first argument to the path
        printf("%s\n", __path);
    }
    
    //execute the command using execv
    if(execvp(__path, args) == -1){
        //handle error
        perror("execv");
    }

    //free memory if allocated
    if(args[0][0] != '/'){
        free(__path);
    }
}



// Define shell_loop()
int shell_loop() {
    printf("$:");
    while(1) {
        // Print shell prompt and read input
        char *command = NULL;
        size_t size = 0;
        getline(&command, &size, stdin);
    
        // Code to exit the loop
        if (strcmp(command, "exit\n") == 0 || strcmp(command, "Exit\n") == 0 || strcmp(command, "EXIT\n") == 0) { 
            free(command);
            return 0; 
        }

        // Parse command into arguments
        char **args;
        args = parse_command(command);
        free(command); // Free the memory
        

        // Creating a child process and executing command
        int rc = fork();
        if (rc < 0){
            // Error handling for fork failure
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (rc == 0){
            // Code for child process
            if (DEBUG == 1) {
                printf("Pid:%d Type: Child\n", (int) getpid());
            }
            // Execute the command using exec()
            // Binaries are located at '/usr/bin/' directory.
            execute_command(args);
            //return to the parent process
            return 0;
        } else {
            // Code for parent process
            printf("$:");
            //Wait for the child process to end
            int wc = wait(NULL);
            if (DEBUG == 1) { 
                printf("Pid:%d Type: Shell: ", (int) getpid()); 
            }
        }
        free(args);
    }
}

// Define main function
int main(int argc, char *argv[]) {
    // Run the shell loop
    shell_loop();
    return 0;
}
