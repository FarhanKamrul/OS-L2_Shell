#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<string.h>
#include <stdbool.h>

// Define MAXSIZE
int MAXSIZE = 20;
int DEBUG = 0;

// Define parse_command()
char **parse_command(const char *command) {
    char **args = malloc(sizeof(char*) * MAXSIZE);
    if (!args) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    char *token = strtok((char *)command, " ");
    while (token != NULL) {
        
        if (DEBUG == 1){ printf("%s\n", token); }
        args[index] = strdup(token);
        if (!args[index]) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        index++;
        if (index >= MAXSIZE) {
            fprintf(stderr, "Too many arguments\n");
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, " ");
    }
    args[index] = NULL;
    return args;
}

int shell_loop(){
    printf("$:");
    while(1){
        // Print 'shell:>' and wait for input
        //Reading and Parsing Commands
        char *command = NULL;
        size_t size = 0;
        
        getline(&command, &size, stdin);
    
        //Code to exit the loop
        if (strcmp(command, "exit\n") == 0 || strcmp(command, "Exit\n") == 0 || strcmp(command, "EXIT\n") == 0) { 
            free(command);
            return 0; 
        }

        char **args;
        args = parse_command(command);
        free(command); // Free the memory
        free(args);

        // Creating a child process and executing command
        int rc = fork();

        if (rc < 0){
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (rc == 0){
            if (DEBUG == 1){
                printf("Pid:%d Type: Child\n", (int) getpid());
            }
            //execute the command using exec()
            

        } else {
            printf("$:");
            //int wc = wait(NULL);
            if (DEBUG == 1){ printf("Pid:%d Type: Shell\n", (int) getpid()); }
            
        }
    }
}


int main(int argc, char *argv[]){

    // Run the shell loop
    shell_loop();
    return 0;
}
