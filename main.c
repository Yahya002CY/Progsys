#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_SIZE 128
#define WELCOME_MSG "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define PROMPT "enseash % "
#define BYE_MSG "Bye bye...\n"

int main() {
    char command[MAX_SIZE];
    int status;
    ssize_t ret;
    pid_t pid;

    // Display welcome message
    write(STDOUT_FILENO, WELCOME_MSG, strlen(WELCOME_MSG));

    while (1) {
        // Display prompt
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        // Read user input
        ret = read(STDIN_FILENO, command, MAX_SIZE);

        // --- PARTIE AJOUTÉE POUR Q3 ---
       
        // 1. Handle <ctrl>+d
        // If read returns 0, it means End Of File (EOF)
        if (ret == 0) {
            write(STDOUT_FILENO, BYE_MSG, strlen(BYE_MSG));
            break; // Exit the loop
        }

        // Replace the newline '\n' with '\0'
        command[ret - 1] = '\0';

        // 2. Handle "exit" command
        // We check if the command is "exit" AND if the length matches (5 bytes including \n)
        if (strncmp(command, "exit", 4) == 0 && ret == 5) {
            write(STDOUT_FILENO, BYE_MSG, strlen(BYE_MSG));
            break; // Exit the loop
        }
       
        // ------------------------------

        // Execution (Standard fork/exec/wait)
        pid = fork();
        if (pid == 0) {
            execlp(command, command, (char *)NULL);
            // If we are here, exec failed
            char *error_msg = "Command not found\n";
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            exit(EXIT_FAILURE);
        } else {
            wait(&status);
        }
    }
    return 0;
}
