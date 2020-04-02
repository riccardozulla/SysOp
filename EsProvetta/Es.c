
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



unsigned long fibonacci (unsigned int n);
unsigned long * fibonacci_array(unsigned int n);
void array_reverse(unsigned long * array, int size);
void bubble_sort(unsigned long * array, int n);
void swap_long(unsigned long * a, unsigned long * b);
void print_array(unsigned long * array, int length);

//A - scrivere la seguente funzione:
//
//implementare una funzione che calcola la serie di Fibonacci:
//
//unsigned long fibonacci(unsigned int n);
//
//dove:
//fibonacci(0) = 0
//fibonacci(1) = 1
//fibonacci(n) = fibonacci(n-1) + fibonacci(n-2)  per n > 1
//
//provare a scoprire quale è il valore massimo di n per cui fibonacci(n) restituisce un valore della serie corretto

unsigned long fibonacci (unsigned int n){

	if (n==0) return 0;
	else if (n==1) return 1;
	else return fibonacci(n-1)+fibonacci(n-2);
}

//B - scrivere la seguente funzione:
//
//unsigned long * fibonacci_array(unsigned int n);
//
//questa funzione restituisce un array di unsigned long di dimensione n+1 (n è l'argomento della funzione)
//dove la cella i-ma contiene il valore di fibonacci(i).

unsigned long * fibonacci_array(unsigned int n){

	unsigned int size = n+1;
	unsigned long * array;
	array = calloc(size,sizeof(unsigned long));

	if(array == NULL){
		printf("calloc ha fallito!");
		exit(1);

	}


	for (unsigned int i=0; i<size; i++){
		array[i]=fibonacci(i);
	}
	return array;
}


//C - main() fa queste operazioni:
//
//calcola fibonacci_array(39) e lo assegna alla varibile 'fibonacci_result'  // fibonacci(39) = 63245986
//
//fare una copia dell'array fibonacci_result in 'fibonacci_result_copy' (i due array devono avere aree di memoria distinte)
//
//fibonacci_result_copy: fare il 'reverse' dell'array ('copiare' da esempio 004.1arrays-operations)
//
//su fibonacci_result_copy: applicare bubble_sort ('copiare' da esempio 004.1arrays-operations)
//
//confrontare fibonacci_result e fibonacci_result_copy, verificare che i due array siano uguali (scrivere su stdout il risultato).

int main (){

	unsigned long * fibonacci_result;
	unsigned long * fibonacci_result_copy;

	fibonacci_result = fibonacci_array(10); //array di fibonacci(10)
	int size =11;

	printf("Il risultato della funzione fibonacci(10) è: %lu\n", fibonacci_result[10]);


	//fibonacci_result_copy = make_copy_of_array(fibonacci_result, size);

	fibonacci_result_copy= malloc (size*sizeof(unsigned long));
	if(fibonacci_result_copy == NULL){
			printf("malloc ha fallito!");
			exit(EXIT_FAILURE);
		}

	memcpy(fibonacci_result_copy, fibonacci_result, size*sizeof(unsigned long));


	printf("Creata nuova copia di fibonacci_result:\n");
	print_array(fibonacci_result_copy, size);


	array_reverse(fibonacci_result_copy, size);

	printf("L'array dopo la funzione array_reverse:\n");
	print_array(fibonacci_result_copy, size);

	bubble_sort(fibonacci_result_copy, size);

	printf("L'array dopo la funzione bubble_sort:\n");
	print_array(fibonacci_result_copy, size);









	 int result = memcmp(fibonacci_result, fibonacci_result_copy, size*sizeof(unsigned long));


	 if (result==0) printf("I due array sono uguali!");
	 else printf("HAI SBAGLIATO: SONO DIVERSI");


	free(fibonacci_result);
	free(fibonacci_result_copy);

	exit(EXIT_SUCCESS);

}







void array_reverse(unsigned long * array, int size){

	unsigned long mem;
	for(int i=0; i<size/2; i++){
		mem=array[i];
		array[i]=array[(size-1)-i];
		array[(size-1)-i]=mem;
	}
}

void bubble_sort(unsigned long * array, int n){

	while(n>1){
		int newn=0;

		for(int i=1; i<n; i++){
			if(array[i-1]>array[i]){
				swap_long(&array[i-1],&array[i]);
				newn=i;
			}
		}
		n=newn;
	}
}




void swap_long(unsigned long * a, unsigned long * b){
	unsigned long mem=*a;
	*a=*b;
	*b=mem;
}


void print_array(unsigned long * array, int length){
	for(int i=0; i<length; i++){
		printf("array[%u]\t%lu\n", i, array[i]);
	}
	printf("\n");
}
