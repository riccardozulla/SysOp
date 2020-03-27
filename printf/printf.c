#include <stdio.h>

int main(int argc, char **argv) {

		char op;
		int n1 = 0;
		int n2 = 0;
		int res = 0;
		int control;

		printf("Inserire l'operazione da eseguire nel seguiente formato:\noperatore\tprimo numero\tsecondo numero\n");
		printf("dove: operazione = +,-,*,/\n");
		printf("Inserire valori: ");
		control = scanf("%c %d %d", &op, &n1, &n2);

		if (control == 3) {

			switch(op) {
					case '+':
						res = n1 + n2;
						break;
					case '-':
						res = n1 - n2;
						break;
					case '*':
						res = n1 * n2;
						break;
					case '/':
						res = n1 / n2;
						break;
					default:
						return 1;
						break;
			}

			printf("%d %c %d = %d", n1, op, n2, res);

		} else {
			printf("Errore: scanf ha resituito %d", control);
		}

	return 0;
}
