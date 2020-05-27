/*
è data la string di caratteri content:

char * content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Mattis rhoncus urna neque viverra justo nec ultrices. Pretium quam vulputate dignissim suspendisse in est ante. Vitae congue mauris rhoncus aenean. Blandit cursus risus at ultrices mi. Ut lectus arcu bibendum at varius vel pharetra vel. Etiam non quam lacus suspendisse faucibus interdum posuere. Eget sit amet tellus cras adipiscing enim eu turpis egestas. Lectus magna fringilla urna porttitor rhoncus dolor purus non. Sit amet consectetur adipiscing elit duis tristique sollicitudin nibh. Nec tincidunt praesent semper feugiat nibh. Sapien pellentesque habitant morbi tristique senectus et netus et malesuada.";

il programma utilizza due thread aggiuntivi, il primo thread analizza
i caratteri nelle posizioni "pari" della stringa (content[0], content[2], content[4]...)
mentre il secondo considera i caratteri nelle posizioni "dispari" della stringa
(content[1], content[3], content[5]...)

definire un array condiviso:

int counter[256];

ciascun thread incrementa il contatore (counter) corrispondente al valore di ogni carattere incontrato:

counter[content[i]]++;

usare un solo semaforo per risolvere il problema (ci sono più modi di risolverlo...)

il thread principale aspetta il termine dei due thread e poi scrive su stdout la frequenza di ogni carattere.

"frequenza %d = %d\n"
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <errno.h>
#include <pthread.h>


int counter[256];
char * content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Mattis rhoncus urna neque viverra justo nec ultrices. Pretium quam vulputate dignissim suspendisse in est ante. Vitae congue mauris rhoncus aenean. Blandit cursus risus at ultrices mi. Ut lectus arcu bibendum at varius vel pharetra vel. Etiam non quam lacus suspendisse faucibus interdum posuere. Eget sit amet tellus cras adipiscing enim eu turpis egestas. Lectus magna fringilla urna porttitor rhoncus dolor purus non. Sit amet consectetur adipiscing elit duis tristique sollicitudin nibh. Nec tincidunt praesent semper feugiat nibh. Sapien pellentesque habitant morbi tristique senectus et netus et malesuada.";

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void print_risultato(){
	for (int i=0; i<254; i++){
		printf("counter[%d]\t(%c) = %d\n", i, (char)i, counter[i]);
	}
}

void *thread_function_pari(){
	for(int i=0; i<strlen(content);i=i+2){
		int s;

		s=pthread_mutex_lock(&mutex);
		if(s!=0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

		counter[(int)content[i]]++;

		s=pthread_mutex_unlock(&mutex);
		if(s!=0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}


	}
	return NULL;
}

void *thread_function_dispari(){
	for(int i=1; i<strlen(content);i=i+2){
		int s;

		s=pthread_mutex_lock(&mutex);
		if(s!=0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

		counter[(int)content[i]]++;

		s=pthread_mutex_unlock(&mutex);
		if(s!=0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}


	}
	return NULL;
}


int main(){
	pthread_t t1;
	pthread_t t2;

	int s;

	s = pthread_create(&t1, NULL, thread_function_pari, NULL);

	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	s = pthread_create(&t2, NULL, thread_function_dispari, NULL);

	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}



	s = pthread_join(t1, NULL);

	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(t2, NULL);

	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	print_risultato();
	exit(EXIT_FAILURE);

}













