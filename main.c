#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_SIZE 128
#define Welcome_msg "Bienvenue dans le shell ENSEA.\n Pour Quitter, tapez 'exit'.\n"
#define Prompt "enseah % .\n"

int main() {
         char command[MAX_SIZE];
       int status;
       ssize_t ret;
       pid_t pid;
  write(STDOUT_FILENO,Welcome_msg,strlen(Welcome_msg));
  while(1){
        write(STDOUT_FILENO,Prompt,strlen(Prompt));
      ret=read(STDIN_FILENO,command, MAX_SIZE);
        
        if(ret <=0) {
            break;
         }  
        command[ret - 1] ='\0';
        pid = fork();       
        if (pid == 0) {
            // Child process code
            // execlp searches for the command in the PATH (like /bin/ls)
            execlp(command, command, (char *)NULL);
           
            // If execlp returns, it means the command failed 
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
