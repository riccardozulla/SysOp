/*
riprendere l'esercizio https://github.com/marcotessarotto/esercizio-C-2020-03-30/

calcolare fibonacci_array(39) e scrivere i valori dell'array in formato testo in un file chiamato fibonacci.txt

se il file esiste, deve essere sovrascritto

il programma scrive nel file un valore (in formato testo) per riga


per convertire un numero intero in stringa, vedere:

https://github.com/marcotessarotto/exOpSys/tree/master/003.2sprintf
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char * argv[]){

char * file_name="fibonacci.txt";

int fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);

	/*
	S_IRWXU  00700 user (file owner) has read, write, and execute permission
	S_IRUSR  00400 user has read permission
	S_IWUSR  00200 user has write permission
	S_IXUSR  00100 user has execute permission
	 */

	if (fd == -1) { // errore!

		perror("open()");

		exit(EXIT_FAILURE);
	}

	if(dup2(fd, STDOUT_FILENO)==-1){
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	if (close(fd) == -1) {
			perror("close");
			exit(EXIT_FAILURE);
	}
	char * newargv[] = {"esercizio-C-2020-03-30", NULL};
	char * newenviron[] = { NULL };

	execve("./../../EsProvetta/Debug/esercizio-C-2020-03-30", newargv, newenviron);

	perror("execve");
	exit(EXIT_FAILURE);
}

