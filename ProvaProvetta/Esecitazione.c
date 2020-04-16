//Esercitazione pre-provetta 7/04/20

//sono definite tre variabili globali, di tipo stringa di caratteri, inizializzate a:
//
//"prima-"
//
//"seconda-"
//
//"terza!"
//
//i nomi delle tre variabili saranno: str_1, str_2 e str_3
//
//il programma utilizza una variabile locale (si chiama str_new), di tipo stringa di caratteri,
//	per immagazzinare il risultato della concatenazione delle tre stringhe di caratteri puntate da str_1, str_2, str_3.
//
//il programma scrive su stdout la stringa risultato della concatenazione.
//
//


#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>




char *str_1 = "prima-";
char *str_2 = "seconda-";
char *str_3 = "terza!";

int my_strlen3(char * data1, char * data2, char * data3);

int main(){

	char *str_new;
	int length = strlen(str_1)+strlen(str_2)+strlen(str_3);
	str_new = calloc(length, sizeof(char));
	if(str_new==NULL){
		perror("calloc ha fallito");
		exit(EXIT_FAILURE);
	}

	strcat(str_new, str_1);
	strcat(str_new, str_2);
	strcat(str_new, str_3);

	printf("La stringa concatenata è: %s\n", str_new);

	printf("la lunghezza delle stringhe è: %d\n", my_strlen3(NULL, str_1, str_2));

	exit(EXIT_SUCCESS);

}




int my_strlen3(char * data1, char * data2, char * data3){

	int l1, l2, l3;

	if(data1==NULL){
		l1 = 0;
		printf("Warning: la striga data1 è uguale a NULL\n");
	}
	else l1 = strlen(data1);

	if(data2==NULL){
		l2 = 0;
		printf("Warning: la striga data2 è uguale a NULL\n");
	}
	else l2 = strlen(data2);

	if(data3==NULL){
		l3 = 0;
		printf("Warning: la striga data3 è uguale a NULL\n");
	}
	else l3 = strlen(data3);

	return l1+l2+l3;

}






