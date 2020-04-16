//scrivere un programma che legge da stdin una riga alla volta (la riga si considera terminata dal carattere \n).
//
//la lunghezza della riga è arbitraria, non fare ipotesi sulla massima lunghezza di riga.
//[in questo esercizio, occorre immagazzinare l'intera riga ed anche le precedenti].
//
//confrontare la riga appena letta con tutte le precedenti righe lette:
//il confronto parte dalla penultima riga e va a ritroso fino alla prima riga letta.
//
//se la riga appena letta ha un precedente, scrivere su stdout il messaggio
//"***questa riga è stata già letta %d righe fa***", dove viene indicata la posizione relativa della riga identica rispetto alla riga attuale
//(può accadere che una riga si ripresenti più di una volta; scrivere tutte le volte
//che si ripresenta)
//
//il programma termina in caso di EOF su stdin.
//
//esempio, I: significa input proveniente da stdin, O: indica l'output del programma su stdout
//
//I: xyz0000
//I: 1234
//I: 0000
//I: zzzzz
//I: 1234
//O: ***questa riga è stata già letta 3 righe fa***
//I: abcdefg
//I: 1234
//O: ***questa riga è stata già letta 2 righe fa***
//O: ***questa riga è stata già letta 5 righe fa***


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_CHARS 51
#define MAX_ARR 10
int riga=0;




void check(char array[MAX_ARR][MAX_CHARS]);

int main(){


	int res;
	char  line[MAX_ARR][MAX_CHARS];
	//line = calloc(MAX_CHARS, sizeof(char));




	while ((res=scanf("%20s", line[riga]))!=EOF){

		if (res < 1) {
				printf("scanf returns %d\n", res);
				exit(EXIT_FAILURE);
			}
			else {
				printf("ok stringa di caratteri ricevuta.\n");
			}

		printf("Stampiamo la riga num: %d\t%s\n",riga,line[riga]);

		check(line);

		riga++;


	}

	printf("Il programma sta terminando\nBye bye\n");




//
//
//	printf("\n");
//	printf("\n");
//
//	unsigned int string_length = strlen(line); // strlen è definita in string.h
//
//	printf("line='%s'\nstrlen(line)=%u\n", line, string_length);
//
//	printf("\n");
//



/*
	   char s[] = "\n"; // caratteri separatori da utilizzare per la suddivisione
	   char *token;

	   // ottieni il primo token
	   token = strtok(line, s);

	   // "cammina" attraverso gli altri token
	   while (token != NULL ) {
	      printf( "%s\n", token );

	      token = strtok(NULL, s); // le chiamate successive a strtok
	   }
*/

}

void check(char array[MAX_ARR][MAX_CHARS]){
	int cont=0;
	int res;
	for (int i=riga-1; i>=0; i--){
		res = strcmp(array[riga], array[i]);
		cont++;
		if(res==0) printf("***questa riga è stata già letta %d righe fa***\n", cont);
	}

}



















