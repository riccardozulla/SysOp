/*
scrivere un programma in C:

è data la stringa di caratteri:

char * text_to_write = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. At ultrices mi tempus imperdiet nulla malesuada pellentesque elit. Non enim praesent elementum facilisis leo vel fringilla est ullamcorper. Non quam lacus suspendisse faucibus interdum. Metus vulputate eu scelerisque felis imperdiet. At ultrices mi tempus imperdiet nulla malesuada pellentesque. At tempor commodo ullamcorper a lacus vestibulum. Consequat semper viverra nam libero justo laoreet sit amet. Facilisis magna etiam tempor orci eu. Convallis tellus id interdum velit laoreet id donec ultrices. Interdum velit euismod in pellentesque massa placerat duis ultricies. Senectus et netus et malesuada fames. Pharetra vel turpis nunc eget lorem dolor. Nisi porta lorem mollis aliquam ut porttitor leo a. Euismod elementum nisi quis eleifend quam adipiscing vitae proin.";

il processo padre crea un file output.txt (se il file esiste già, va troncato a zero e sovrascritto).

il processo padre tiene aperto il file output.txt, salvare il descrittore in una variabile fd

il processo padre crea un processo figlio: l'obiettivo finale è che il file output.txt contenga tutta la stringa text_to_write e che i due processi (processo padre e processo figlio) si dividano il lavoro di scrittura nel file. La scrittura sul file avviene in "parallelo".

il processo figlio apre per conto suo il file output.txt.

il processo padre scrive la prima metà di text_to_write nel file, cominciando a scrivere dalla posizione iniziale del file.

il processo figlio scrive l'altra metà di text_to_write del file, cominciando a scrivere da dove il processo padre ha terminato di scrivere.

il processo padre attende la conclusione del processo figlio e poi termina.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

void figlio(char *text_to_write, int text_len);

int main(){

	char * text_to_write = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. At ultrices mi tempus imperdiet nulla malesuada pellentesque elit. Non enim praesent elementum facilisis leo vel fringilla est ullamcorper. Non quam lacus suspendisse faucibus interdum. Metus vulputate eu scelerisque felis imperdiet. At ultrices mi tempus imperdiet nulla malesuada pellentesque. At tempor commodo ullamcorper a lacus vestibulum. Consequat semper viverra nam libero justo laoreet sit amet. Facilisis magna etiam tempor orci eu. Convallis tellus id interdum velit laoreet id donec ultrices. Interdum velit euismod in pellentesque massa placerat duis ultricies. Senectus et netus et malesuada fames. Pharetra vel turpis nunc eget lorem dolor. Nisi porta lorem mollis aliquam ut porttitor leo a. Euismod elementum nisi quis eleifend quam adipiscing vitae proin.";
	//char * text_to_write = "12345678";
	int text_len = strlen(text_to_write);



	int fd = open("output.txt", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("problema con open");
		exit(EXIT_FAILURE);
	}
	if(ftruncate(fd, text_len)==-1){
		perror("errore ftruncate");
		exit(EXIT_FAILURE);
	}
/*
	if(dup2(fd, STDOUT_FILENO)==-1){
		perror("errore dup");
		exit(EXIT_FAILURE);
	}

	if(close(fd)){
		perror("error close");
		exit(EXIT_FAILURE);
	}
*/

	switch(fork()){
	case -1:
		perror("error fork");
		exit(EXIT_FAILURE);
		break;
	case 0:
		if(close(fd)){
				perror("error close");
				exit(EXIT_FAILURE);
		}
		figlio(text_to_write, text_len);

		printf("F:bye\n");
		break;
	default:
		//padre
		write(fd, text_to_write, (text_len/2));
		if(wait(NULL)==-1){
			perror("wait");
			exit(EXIT_FAILURE);
		}
		printf("P:bye\n");
		break;
	}

exit(EXIT_SUCCESS);
}

void figlio(char *text_to_write, int text_len){
	int fd1 = open("output.txt", O_WRONLY);
	if(fd1 == -1){
		perror("problema con open");
		exit(EXIT_FAILURE);
	}
	if(lseek(fd1, text_len/2, SEEK_SET)==-1){
		perror("error lseek");
		exit(EXIT_FAILURE);
	}
	write(fd1, &text_to_write[text_len/2], text_len/2+1);
}


