	// ESERCIZIO 1:
	// leggi da stdin fino a EOF e conta i caratteri: '\n', '\t' e ' '
	// dopo EOF, scrive su stdout il numero di occorrenze di ciascun carattere




#include <stdio.h>


int main(int argc, char *argv[]) {
	int c;
	unsigned int newLine_counter = 0;
	unsigned int tab_counter = 0;
	unsigned int space_counter = 0;

	while ((c=getchar()) != EOF) {

			switch(c){
			case '\n':
				newLine_counter++;
				break;
			case '\t':
				tab_counter++;
				break;
			case ' ':
				space_counter++;
				break;
			default:
				break;
			}
		}

		printf("\n\nNumero di '\\n'(new line): %u \nNumero di '\\t'(tab): %u \nNumero di ' ' (spazi): %u \n\n", newLine_counter, tab_counter, space_counter);

		return 0;
	}
