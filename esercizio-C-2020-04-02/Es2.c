//il programma svolge le attività sotto descritte fino a quando incontra EOF su stdin.
//
//1: il programma legge caratteri da stdin.
//Quando incontra EOF, il programma scrive quanti processi figli ha lanciato in tutto e poi termina.
//
//Il programma accumula i caratteri letti da stdin in un array di dimensione 80 chiamato char_array.
//(introdurre nel codice un #define NUM_CHARS 80).
//
//Quando char_array è pieno, passa tutto l'array ad un nuovo processo figlio
//che fa delle analisi che scrive su stdout (analisi sotto riportate).
//
//Dopo aver lanciato il processo figlio, il processo principale azzera char_array ed il programma continua da 1:
//
//
//*analisi svolte dai processi figlio (sull'array di 80 caratteri ricevuto)*
//carattere minimo
//carattere massimo
//carattere con più occorrenze
//carattere con meno occorrenze
//
//fatta l'analisi e scritto i risultati , il processo figlio termina.


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



#define NUM_CHARS 10
int num_child=0;
char array_char[NUM_CHARS];

//carattere minimo
//carattere massimo
//carattere con più occorrenze
//carattere con meno occorrenze


void print_array(char*array, int length);
void gen_child();
void analisi();
void analisi1(); //metodo alternatico per il calcolo delle occorrenze




int main(){


	char c;
	int counter=0;
	memset(array_char, 0, NUM_CHARS * sizeof(char));


	while ((c=getchar())!=EOF) {
		array_char[counter]=c;
		counter++;
		if(counter==NUM_CHARS){
			counter=0;
			num_child++;
			gen_child();
			memset(array_char, 0, NUM_CHARS * sizeof(char));
			sleep(1);
		}

	}

	printf("[parent] Ho creato %d figli", num_child);
	exit(EXIT_SUCCESS);

}



void gen_child(){

	pid_t child_pid;

	switch(child_pid = fork()){
	case 0:
		//figlio

		analisi();
		analisi1();

		exit(0);

		break;
	case -1:
		//errore
		printf("Il processo fork ha fallito");
		exit(EXIT_FAILURE);

		break;
	default:
		//padre

		printf("Ho invocato il processo %d\n", child_pid);

		break;


	}
	return;

}



void analisi(){
	char * array=&array_char;
	int length = NUM_CHARS;
	char min='z';
	char max='a';
	char min_oc;
	char max_oc;
	int num_max_oc=0;
	int num_min_oc=NUM_CHARS;
	int oc[256]={0};

	for(int i=0; i<length; i++){
		if(array[i]<min&&array[i]>='a') min= array[i];
		if(array[i]>max) max= array[i];

		int pos = (int)array[i];
		oc[pos]++;


	}

	for(int i=0; i<256; i++){

		if(oc[i]>num_max_oc){
			num_max_oc=oc[i];
			max_oc=  (char) i;
		}
		if(oc[i]<num_min_oc && oc[i]!=0){
		num_min_oc=oc[i];
		min_oc=  (char) i;
		}

	}

	printf("\nSONO IL FIGLIO NUMERO %d\n", num_child);
	printf("Il carattere minimo all'interno dell'array è: %c\n", min);
	printf("Il carattere massimo all'interno dell'array è: %c\n", max);
	printf("La lettera più ripetura è: %c\tTotale ripetizioni: %d\n", max_oc, num_max_oc);
	printf("La lettera meno ripetura è: %c\tTotale ripetizioni: %d\n", min_oc, num_min_oc);

return;
}


void analisi1(){

	char * array=&array_char;
	int length = NUM_CHARS;

	char copy[length];

	for(int i=0; i<length-1; i++){

		if(copy[i]!=-1){
			copy[i]=1;
			for(int j=i+1; j<length; j++){
				if(array[i]==array[j]){
					copy[i]++;
					copy[j]=-1;
				}
			}

		}
	}

	int max=0;
	int min=length;
	int pos_max, pos_min;
	for(int i=0; i<length; i++){
		if(copy[i]!=-1){
			if(copy[i]>max) {max=copy[i]; pos_max=i;}
			if(copy[i]<min) {min=copy[i]; pos_min=i;}
		}

	}


	printf("La lettera più ripetura è: %c\tTotale ripetizioni: %d\n", array[pos_max], max);
	printf("La lettera meno ripetura è: %c\tTotale ripetizioni: %d\n", array[pos_min], min);



}





void print_array(char*array, int length){
	for(int i=0; i<length;i++){
		printf("%c\n", array[i]);
	}
}







