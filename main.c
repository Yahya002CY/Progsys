#include <unistd.h>
#include <string.h>

#define Welcome_msg "Bienvenue dans le shell ENSEA.\n Pour Quitter, tapez 'exit'.\n"
#define Prompt "enseah % .\n"

int main() {

write(STDOUT_FILENO,Welcome_msg,strlen(Welcome_msg));
write(STDOUT_FILENO,Prompt,strlen(Prompt));

 return 0;

}
