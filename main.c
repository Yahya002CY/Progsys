#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_SIZE 128
#define WELCOME_MSG "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define PROMPT "enseash % "

int main() {
    char command[MAX_SIZE];
    int status;
    ssize_t ret;
    pid_t pid;

    // 1. Display welcome message
    write(STDOUT_FILENO, WELCOME_MSG, strlen(WELCOME_MSG));

    // Start the infinite loop
    while (1) {
        // 2. Display prompt
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

        // 3. Read user input
        // We read from STDIN (0) into the buffer 'command'
        ret = read(STDIN_FILENO, command, MAX_SIZE);

        // If read fails or implies Ctrl+D (ret = 0), we stop (for now)
        if (ret <= 0) {
            break;
        }

        // 4. Remove the '\n' (newline character)
        // When you press Enter, 'read' saves the '\n'.
        // We must replace it with '\0' for execlp to work.
        command[ret - 1] = '\0';

        // 5. Execute the command
        pid = fork();

        if (pid == 0) {
            // Child process code
            // execlp searches for the command in the PATH (like /bin/ls)
            execlp(command, command, (char *)NULL);
           
            // If execlp returns, it means the command failed (e.g., typo)
            char *error_msg = "Command not found\n";
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            exit(EXIT_FAILURE);
        } else {
            // Parent process code
            // Wait for the child to finish before showing the prompt again
            wait(&status);
        }
    }

    return 0;
}
