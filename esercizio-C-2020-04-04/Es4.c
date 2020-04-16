//scrivere un programma che legge da stdin una riga alla volta (la riga si considera terminata dal carattere \n).
//
//la lunghezza della riga è arbitraria, non fare ipotesi sulla massima lunghezza di riga.
//[in questo esercizio, non occorre immagazzinare l'intera riga].
//
//calcolare la media della lunghezza (=numero di caratteri) delle righe lette che vengono lette da stdin.
//
//dopo ogni riga letta, mostrare la lunghezza della riga e la media "parziale" fino a quel momento.
//
//il programma termina in caso di EOF su stdin.


#include <stdio.h>
#include <stdlib.h>
#include <float.h>

unsigned int num_righe=0;
unsigned int num_caract=0;
float media=0;


int main(){
	char c = getchar();
	while(c!=EOF){
		c= getchar();
		num_caract=0;
		while(c!='\n'&&c!=EOF){
			num_caract++;
			c= getchar();
		}
		num_righe++;
		media=(media+(num_caract-media)/num_righe);
		printf("La riga letta contiene %d caratteri\nMedia parziale: %.2f\n", num_caract, media);

	}


	printf("Hai scritto %u righe\n", num_righe);

}
