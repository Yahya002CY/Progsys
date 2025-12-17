#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_SIZE 128
#define WELCOME_MSG "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define BYE_MSG "Bye bye...\n"

void print_msg(const char *msg) {
    write(STDOUT_FILENO, msg, strlen(msg));
}

int main() {
    char command[MAX_SIZE];
    char prompt[MAX_SIZE] = "enseash% ";
    int status;
    ssize_t ret;
    struct timespec start, end;
    pid_t pid;

    print_msg(WELCOME_MSG);

    while (1) {
        print_msg(prompt);

        ret = read(STDIN_FILENO, command, MAX_SIZE);

        /* Handle Ctrl+D */
        if (ret == 0) {
            print_msg(BYE_MSG);
            break;
        }

        /* Remove newline */
        command[ret - 1] = '\0';

        /* Handle exit */
        if (strcmp(command, "exit") == 0) {
            print_msg(BYE_MSG);
            break;
        }

        clock_gettime(CLOCK_MONOTONIC, &start);

        pid = fork();

        if (pid == 0) { /* Child */
            char *args[64];
            int i = 0;

            char *token = strtok(command, " ");
            while (token != NULL) {
                args[i++] = token; //Store the current token pointer in the array
                token = strtok(NULL, " ");  //get the next token from the same string 
            }
            args[i] = NULL;// the argument must be terminated by null so execvp knows where it ends.

            execvp(args[0], args); // 'v' takes a vector (array) of arguments,'p' searches for the executable in the system PATH variable

            /* Exec failed */
            print_msg("Command not found\n");
            exit(EXIT_FAILURE);

        } else { /* Parent */
            wait(&status);

            clock_gettime(CLOCK_MONOTONIC, &end);

            long elapsed_ms =
                (end.tv_sec - start.tv_sec) * 1000 +
                (end.tv_nsec - start.tv_nsec) / 1000000;

            if (WIFEXITED(status)) {
                snprintf(prompt, sizeof(prompt),
                         "enseash [exit:%d|%ldms]%% ",
                         WEXITSTATUS(status), elapsed_ms);
            } else if (WIFSIGNALED(status)) {
                snprintf(prompt, sizeof(prompt),
                         "enseash [sign:%d|%ldms]%% ",
                         WTERMSIG(status), elapsed_ms);
            }
        }
    }

    return 0;
}

