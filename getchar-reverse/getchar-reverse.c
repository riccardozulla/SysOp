	// ESERCIZIO 2:

	// scrivere il seguente programma:

	// legge e salva tutti i dati provenienti da stdin fino a EOF

	// poi scrive su stdout i dati accumulati ma all'incontrario (dall'ultimo al primo)

	// suggerimenti: .....


#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

	char c;
	char * array;
	int size = 1;
	array = calloc(size, sizeof(char));

	while((c=getchar()) != EOF){
		array[size-1]=c;

		array=realloc(array, (size++)*sizeof(char));
	}

	swap_char(array, size);

	printf("Questo è l'array-reverse:\n");

	for (int i=0; i<size; i++){
		printf("%c", array[i]);
	}
	printf("\n");



	return 0;
}

void swap_char(char * array, int size){
	char mem;
	for(int i=0; i<size/2; i++){
		mem=array[i];
		array[i]=array[(size-1)-i];
		array[(size-1)-i]=mem;
	}
}
