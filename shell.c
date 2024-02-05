#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

// Define MAXSIZE
int MAXSIZE = 20;

// Define ANSI escape codes for color formatting
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_GREEN "\x1b[92m"

#define ANSI_COLOR_RESET "\x1b[0m"

// Define parse_command()
char **parse_command(char *command) {
    // Remove the newline character at the end of the command
    size_t len = strlen(command);
    if (len > 0 && command[len-1] == '\n') {
        command[--len] = '\0';
    }
    // Allocate memory for arguments
    char **args = malloc(sizeof(char*) * MAXSIZE);
    if (!args) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    char *token = strtok(command, " ");
    while (token != NULL) {
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
    //execute the command using execv
    if(execvp(args[0], args) == -1){
        //handle error
        perror("execv");
    }
}



// Define shell_loop()
int shell_loop() {
    while(1) {
        // Print current directory
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf(ANSI_COLOR_GREEN "Basic_shell: " ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "$: ", cwd);
        } else {
            perror("getcwd() error");
            return 1;  // Exit loop on error
        }
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
            // Execute the command using exec()
            // Binaries are located at '/usr/bin/' directory.
            execute_command(args);

            printf("execute_command() did not work \n");
            //return to the parent process
            return 0;
        } else {
            // Code for parent process
            //Wait for the child process to end
            int wc = wait(NULL);
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



