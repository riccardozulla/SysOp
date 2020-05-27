/*
 prendere la stringa di caratteri:

char * content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Mattis rhoncus urna neque viverra justo nec ultrices. Pretium quam vulputate dignissim suspendisse in est ante. Vitae congue mauris rhoncus aenean. Blandit cursus risus at ultrices mi. Ut lectus arcu bibendum at varius vel pharetra vel. Etiam non quam lacus suspendisse faucibus interdum posuere. Eget sit amet tellus cras adipiscing enim eu turpis egestas. Lectus magna fringilla urna porttitor rhoncus dolor purus non. Sit amet consectetur adipiscing elit duis tristique sollicitudin nibh. Nec tincidunt praesent semper feugiat nibh. Sapien pellentesque habitant morbi tristique senectus et netus et malesuada.";

il processo crea tre threads; passa la stringa di caratteri content come parametro a ciascuno dei tre threads.

il primo thread conta il numero di vocali nella stringa e restituisce questo valore.

il secondo thread conta il numero di consonanti nella stringa e restituisce questo valore.

il terzo thread conta il numero di spazi e newline nella stringa e restituisce questo valore.

il thread principale aspetta il risultato dei tre threads e poi scrive su stdout i risultati.
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

int check_vocali (char c){
	char*vocali ="aeiouAEIOU";
	for(int i=0; i<strlen(vocali); i++){
		if(c==vocali[i]) return 1;
	}
	return 0;
}

int check_consonanti(char c){
	char * consonanti = "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ";
	for(int i=0; i<strlen(consonanti); i++){
		if(c==consonanti[i]) return 1;
	}
	return 0;
}


void * thread_function_vocali(void * void_str){
	int *res=calloc(1, sizeof(int));
	if(res==NULL){
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	char*str=(char*)void_str;
	for(int i=0; i<strlen(str); i++){
		if(check_vocali(str[i])){
			(*res)++;
		}

	}
	printf("res= %d\n", *res);
	return res;
}

void * thread_function_consonanti(void * void_str){

	int *res=calloc(1, sizeof(int));
	if(res==NULL){
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	char*str=(char*)void_str;
	for(int i=0; i<strlen(str); i++){
		if(check_consonanti(str[i])){
			(*res)++;
		}
	}
	return res;
}

void * thread_function_spazi(void * void_str){
	int *res=calloc(1, sizeof(int));
	if(res==NULL){
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	char*str=(char*)void_str;
	for(int i=0; i<strlen(str); i++){
		if(str[i]==' ' || str[i]=='\n'){
			(*res)++;
		}
	}
	return res;
}



#define THREAD_CHECK(a, msg) {if (s != 0) {perror("pthread_create"); exit(EXIT_FAILURE);}}

int main(int argc, char * argv[]) {

	char * content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Mattis rhoncus urna neque viverra justo nec ultrices. Pretium quam vulputate dignissim suspendisse in est ante. Vitae congue mauris rhoncus aenean. Blandit cursus risus at ultrices mi. Ut lectus arcu bibendum at varius vel pharetra vel. Etiam non quam lacus suspendisse faucibus interdum posuere. Eget sit amet tellus cras adipiscing enim eu turpis egestas. Lectus magna fringilla urna porttitor rhoncus dolor purus non. Sit amet consectetur adipiscing elit duis tristique sollicitudin nibh. Nec tincidunt praesent semper feugiat nibh. Sapien pellentesque habitant morbi tristique senectus et netus et malesuada.";


	pthread_t t1;
	pthread_t t2;
	pthread_t t3;
	void * res;

	int s;


	s = pthread_create(&t1, NULL, thread_function_vocali, (void *)content);
	THREAD_CHECK(s, "pthread_create")

	s = pthread_create(&t2, NULL, thread_function_consonanti, (void *)content);
	THREAD_CHECK(s, "pthread_create")

	s = pthread_create(&t3, NULL, thread_function_spazi, (void *)content);
	THREAD_CHECK(s, "pthread_create")


	printf("strlen = %ld\n", strlen(content));
	int tot=0;

	s = pthread_join(t1, &res);
	THREAD_CHECK(s, "pthread_join")

	printf("vocali = %d\n", *((int *)res));
	tot=tot+*((int *)res);

	s = pthread_join(t2, &res);
	THREAD_CHECK(s, "pthread_join")

	printf("consonanti = %d\n", *((int *)res));
	tot=tot+*((int *)res);

	s = pthread_join(t3, &res);
	THREAD_CHECK(s, "pthread_join")

	printf("spazi = %d\n", *((int *)res));
	tot=tot+*((int *)res);

	printf("totale = %d", tot);


	printf("bye\n");

	return 0;
}
