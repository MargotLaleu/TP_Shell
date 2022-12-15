#include "main.h"

#define WELCOME "./enseash \n Bienvenue dans le Shell ENSEA. \n Pour quitter, tapez 'exit'\n"
#define PROMPT "enseash % \n"
#define TAILLE 128
#define EXIT "Bye Bye ... \n"

char ComandLine[TAILLE];
ssize_t cmd_size;
pid_t pid;
int status;
pid_t finpid;


int main(){
		
	write(STDIN_FILENO , WELCOME, strlen(WELCOME));
			
	while(1){
		
		write(STDIN_FILENO , PROMPT, strlen(PROMPT));
		cmd_size = read(STDOUT_FILENO, ComandLine, TAILLE );
		ComandLine[cmd_size-1] = '\0';
				
		if (strcmp(ComandLine, "exit") == 0 || cmd_size == 0){
			
			write(STDIN_FILENO , EXIT, strlen(EXIT));
			exit(1);
		}
		
		pid = fork();
		if(pid != 0){
			finpid = wait(&status);
		}	
		else{
			execlp(ComandLine, ComandLine, (char*) NULL);
			
		}
				
	}
				
	return EXIT_SUCCESS;	
}
