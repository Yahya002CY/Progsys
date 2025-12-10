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

    
    write(STDOUT_FILENO, WELCOME_MSG, strlen(WELCOME_MSG));

    
    while (1) {
        
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

     
        ret = read(STDIN_FILENO, command, MAX_SIZE);

      
        if (ret <= 0) {
            break;
        }

     
        command[ret - 1] = '\0';

      
        pid = fork();

        if (pid == 0) {
          
            execlp(command, command, (char *)NULL);
           
         
            char *error_msg = "Command not found\n";
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            exit(EXIT_FAILURE);
        } else {
            
            wait(&status);
        }
    }

    return 0;
}
