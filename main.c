#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_SIZE 128
#define WELCOME_MSG "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define BYE_MSG "Bye bye...\n"


void print_msg(const char *msg){
     write(STDOUT_FILENO,msg ,strlen(msg));
}

int main() {
    char command[MAX_SIZE];
    char prompt[MAX_SIZE]="enseash%";
    int status;
    ssize_t ret;
    struct timespec start,end;
    pid_t pid;
    

    print_msg(WELCOME_MSG);

    while (1) {
        // Display prompt
        print_msg(prompt);

        // Read user input
        ret = read(STDIN_FILENO, command, MAX_SIZE);

        // --- PARTIE AJOUTÉE POUR Q3 ---
       
        // 1. Handle <ctrl>+d
        // If read returns 0, it means End Of File (EOF)
        if (ret == 0) {
           print_msg(BYE_MSG);
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
        
	clock_gettime(CLOCK_MONOTONIC,&start);
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

	clock_gettime(CLOCK_MONOTONIC,&end);
	long elapsed_ms=(end.tv_sec-start.tv_sec)*1000 +(end.tv_nsec - start.tv_nsec)/1000000 ;
         
        //WIFEXITED checks for normal termination.
	//WIFSIGNALED checks if the process was killed by a signal
 	if(WIFEXITED(status)){
 	snprintf(prompt,sizeof(prompt),"enseash [exit: %d|%ldms]%%",WEXITSTATUS(status),elapsed_ms);
     }  else if (WIFSIGNALED(status)){
 	snprintf(prompt,sizeof(prompt),"enseash[sign:%d|%ldms]%%",WTERMSIG(status),elapsed_ms);
     }      
     
    }
    return 0;
}
