#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>

#define MAX_SIZE 128
#define WELCOME_MSG "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define BYE_MSG "Bye bye...\n"

void print_msg(const char *msg) {
    write(STDOUT_FILENO, msg, strlen(msg));
}

int main() {
    char command[MAX_SIZE];
    char prompt[MAX_SIZE] = "enseash % ";
    int status;
    ssize_t ret;
    struct timespec start, end;
    pid_t pid;

    print_msg(WELCOME_MSG);

    while (1) {
        print_msg(prompt);

        ret = read(STDIN_FILENO, command, MAX_SIZE);

        // Handle Ctrl+D (End of File)
        if (ret == 0) {
            print_msg(BYE_MSG);
            break;
        }

        // Remove the trailing newline character
        command[ret - 1] = '\0';

        // Handle the built-in 'exit' command
        if (strcmp(command, "exit") == 0) {
            print_msg(BYE_MSG);
            break;
        }

        // Record start time for performance tracking
        clock_gettime(CLOCK_MONOTONIC, &start);

        pid = fork();
        if (pid == 0) { // Child process
            // Variables for handling I/O redirection
            char *input_file = NULL;
            char *output_file = NULL;
            char *args[64];
            int i = 0;
           
            // Create a copy of the command for parsing
            char cmd_copy[MAX_SIZE];
            strcpy(cmd_copy, command);
           
            // Tokenize string and identify redirection symbols
            char *token = strtok(cmd_copy, " ");
            while (token != NULL) {
                if (strcmp(token, "<") == 0) {
                    // Input redirection detected
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        input_file = token;
                    }
                } else if (strcmp(token, ">") == 0) {
                    // Output redirection detected
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        output_file = token;
                    }
                } else {
                    // Standard command argument
                    args[i++] = token;
                }
                token = strtok(NULL, " ");
            }
            args[i] = NULL; // NULL-terminate the arguments array
           
            // Handle input redirection logic
            if (input_file != NULL) {
                int fd_in = open(input_file, O_RDONLY);
                if (fd_in < 0) {
                    perror("Error opening input file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd_in, STDIN_FILENO); // Redirect STDIN to file
                close(fd_in);
            }
           
            // Handle output redirection logic
            if (output_file != NULL) {
                int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd_out < 0) {
                    perror("Error opening output file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd_out, STDOUT_FILENO); // Redirect STDOUT to file
                close(fd_out);
            }
           
            // Execute the external command
            if (args[0] != NULL) {
                execvp(args[0], args);
            }
           
            // execvp only returns if an error occurred
            perror("Command execution failed");
            exit(EXIT_FAILURE);
           
        } else if (pid > 0) { // Parent process
            wait(&status); // Wait for the child process to finish
            clock_gettime(CLOCK_MONOTONIC, &end);

            // Calculate execution time in milliseconds
            long elapsed_ms = (end.tv_sec - start.tv_sec) * 1000 +
                              (end.tv_nsec - start.tv_nsec) / 1000000;

            // Check if child exited normally or via a signal
            if (WIFEXITED(status)) {
                snprintf(prompt, sizeof(prompt),
                         "enseash [exit:%d|%ldms] %% ",
                         WEXITSTATUS(status), elapsed_ms);
            } else if (WIFSIGNALED(status)) {
                snprintf(prompt, sizeof(prompt),
                         "enseash [sign:%d|%ldms] %% ",
                         WTERMSIG(status), elapsed_ms);
            }
        }
    }

    return 0;
}

