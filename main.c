#include "main.h"

#define WELCOME "./enseash \n Bienvenue dans le Shell ENSEA. \n Pour quitter, tapez 'exit'\n"
#define PROMPT "enseash % "
#define TAILLE 128
#define TailleBuff 512
#define EXIT "Bye Bye ... \n"

char ComandLine[TAILLE];
ssize_t cmd_size;
pid_t pid;
int status;
pid_t finpid;
char Buffer[TailleBuff];
struct timespec TimeStart;
struct timespec TimeEnd ;
long duree;
char* PointerLeft;
char* PointerRight;


int main(){
		
	write(STDOUT_FILENO , WELCOME, strlen(WELCOME)); // Affichage du message de Bienvenue
	
			
	while(1){
		
				// GESTION DE LA COMMANDE //
		
		write(STDOUT_FILENO , PROMPT, strlen(PROMPT));  // Affichage du Prompt après chaque exécution de commande
		cmd_size = read(STDIN_FILENO, ComandLine, TAILLE );
		ComandLine[cmd_size-1] = '\0';
		
		if (strcmp(ComandLine, "exit") == 0 || cmd_size == 0){ //Conditions de sortie du terminal
			write(STDOUT_FILENO , EXIT, strlen(EXIT));
			exit(1);
		}
		
		clock_gettime(CLOCK_REALTIME, &TimeStart);  //prélève le temps absolue et le stock
		pid = fork();
		
		if(0 == pid){ // PROCESSUS FILS
			
			if((strstr(ComandLine, ">")) != NULL){      // stocke le retour de la commande dans un fichier
				PointerLeft = strtok(ComandLine, ">");
				PointerRight = strtok(NULL, ">");
						
				int fd_file = open(PointerRight, O_CREAT | O_WRONLY, S_IRWXO | S_IRWXG | S_IRWXU ); 	 
				dup2(fd_file, STDOUT_FILENO);
				strcpy(ComandLine, PointerLeft);
			
			}	
			else if(strstr(ComandLine, "<") != NULL){   // Prend un fichier en argument
			
				PointerLeft = strtok(ComandLine, "<");
				PointerRight = strtok(NULL, "<");
						
				int fd_file = open(PointerRight, O_RDONLY, S_IRWXO | S_IRWXG | S_IRWXU ); 
				dup2(fd_file, STDIN_FILENO);
				strcpy(ComandLine, PointerLeft);
			
			}
							//Séparation fonction et arguments de la commande
			char *Mots[TAILLE] = {NULL};				
			int k =0;						
			Mots[k] = strtok(ComandLine, " ");
			while(Mots[k] != NULL){
				k++;
				Mots[k] = strtok(NULL, " ");
			}	
			
			execvp(Mots[0], Mots);
			exit(123); 	// exécutée si execlp a échoué
		}	
		
			// AFFICHAGE DU PROMPT // 
		
		else{  		  // PROCESSUS PERE
			finpid = wait(&status);
			clock_gettime(CLOCK_REALTIME, &TimeEnd);	
			duree = ( (TimeEnd.tv_sec - TimeStart.tv_sec)*1000 + TimeEnd.tv_nsec - TimeStart.tv_nsec)/1000000;
						
			if(WIFEXITED(status)){ //le fils s'est terminé normalement
				sprintf(Buffer, "enseah [exit : %d | %ld ms] \n", WEXITSTATUS(status), duree);
				write(STDOUT_FILENO, Buffer, strlen(Buffer));
			}
			else if(WIFSIGNALED(status)){ // le fils s'est terminé à cause d'un signal
				sprintf(Buffer, "enseah [signal : %d | %ld ms] \n", WTERMSIG(status), duree);
				write(STDOUT_FILENO, Buffer, strlen(Buffer));
			}						
		}		
	}
				
	return EXIT_SUCCESS;	
}
