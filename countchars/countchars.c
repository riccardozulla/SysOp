#include <stdio.h>

int main (){


	int c;
	unsigned long newLine_counter = 0;
	unsigned long tab_counter = 0;
	unsigned long space_counter = 0;

	while ((c = getchar())!= EOF) {

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

	printf("\nNumero di '\\n': %lu\nNumero di '\\t': %lu\nNumero di ' ' : %lu\n\n", newLine_counter, tab_counter, space_counter);

	return 0;
}
