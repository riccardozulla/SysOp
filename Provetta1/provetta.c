//Riccardo Zulla - IN0500565


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/wait.h>




int work_array[] = {7788, -7066, -6641, -2324, 7799, 3530, -8426, 267, -569, -4432,
		-217, 1761, 4885, 274, -6318, 3527, 4346, 1710, 2562, 7932, -8472, 7351, 354,
		-7463, 1657, -4285, 3926, -7771, -4498, -3430, -5992, 2488, 658, -6352, -7909,
		188, 3005, -8457, 4242, 1259, 3662, 32, 7910, 1252, 3067, -5430, 6794, 3550, -3073,
		2204, -821, 6769, 390, -2097, 1685, -2996, -6410, 3972, -4800, 4674, -2337, 1897, -1160,
		-7987, -8890, -2197, 938, -6302, -5909, 3723, 8615, 7096, 3080, -5511, 6339};


typedef struct {

   int splitter_value; // valore "separatore"

   int * less_than_array; // array dei valori <= splitter_value

   unsigned int less_than_array_len; // lunghezza di less_than_array

   int * larger_than_array; // array dei valori > splitter_value

   unsigned int larger_than_array_len; // lunghezza di larger_than_array

} split_result;


split_result splitter(int * param_array, unsigned int array_len, int splitter_value);
void print_split_result(split_result r);
void child_process(int *array, unsigned int len);


split_result splitter(int * param_array, unsigned int array_len, int splitter_value){
	unsigned int len1=0;
	for(int i=0; i<array_len; i++){
		if(param_array[i]<=splitter_value) len1++;
	}

	int * array1=calloc(len1, sizeof(int));
	if (array1==NULL){
		perror("Errore nella chiamata di calloc");
		exit(EXIT_FAILURE);
	}
	int *array2=calloc(array_len-len1, sizeof(int));
	if (array2==NULL){
		perror("Errore nella chiamata di calloc");
		exit(EXIT_FAILURE);
	}
	int in1=0, in2=0;
	for(int i=0; i<array_len; i++){
		if(param_array[i]<=splitter_value){
			array1[in1]=param_array[i];
			in1++;
		}
		else{
			array2[in2]=param_array[i];
			in2++;
		}

	}


	split_result ris;
	ris.larger_than_array=malloc((array_len-len1)*sizeof(int));
	if (ris.larger_than_array==NULL){
		perror("Errore nella chiamata di malloc");
		exit(EXIT_FAILURE);
	}
	memcpy(ris.larger_than_array, array2, (array_len-len1)*sizeof(int));
	ris.larger_than_array_len=array_len-len1;

	ris.less_than_array=malloc(len1*sizeof(int));
	if (ris.less_than_array==NULL){
		perror("Errore nella chiamata di malloc");
		exit(EXIT_FAILURE);
	}
	memcpy(ris.less_than_array, array1, len1*sizeof(int));
	ris.less_than_array_len=len1;
	ris.splitter_value=splitter_value;

	free(array1);
	free(array2);

	return ris;

}

void print_split_result(split_result r){
	printf("\nI valori inferiori e uguali allo splitter_value: %d sono:\n", r.splitter_value);
	for(int i=0; i<r.less_than_array_len; i++){
		printf("%d ", r.less_than_array[i]);
	}
	printf("\nI valori superiori allo splitter_value: %d sono:\n", r.splitter_value);
	for(int i=0; i<r.larger_than_array_len; i++){
		printf("%d ", r.larger_than_array[i]);
	}


}


void child_process(int *array, unsigned int len){
	int max=-32767, min=32767;
	for(int i=0; i<len; i++){
		if(array[i]<min) min=array[i];
		else if(array[i]>max) max=array[i];
	}
	printf("[child] il valore minimo è: %d\n[child] il valore massimo è: %d\n", min, max);
	exit(EXIT_SUCCESS);
}


int main(){

/*
	int array [] = { 0,1,2,3,4,5,6,7,8,9 };

	split_result r;

	r = splitter(array, 10, 5);

	print_split_result(r);
*/


	//dimensione work_array
	int work_array_len=sizeof(work_array)/sizeof(int);
	printf("L'array work_array ha %d celle.\n", work_array_len);
	//splitter di work_array
	split_result r;
	r = splitter(work_array, work_array_len, 0);
	//stampa di numero celle
	printf("Il numero di celle dell'array less_than_array è: %u\n"
			"Il numero di celle dell'array larger_than_array è: %u\n", r.less_than_array_len, r.larger_than_array_len);
	printf("\n");

	switch (fork()) {
			case 0:
				child_process(r.less_than_array, r.less_than_array_len);
				break;
			case -1:
				perror("fork");
				exit(EXIT_FAILURE);
				break;
			default:
				break;
		}

		printf("[parent] aspetto che il mio primo figlio termini...\n");

		if (wait(NULL) == -1) {
			perror("wait error");
		}
		else
			printf("[parent] terminato il primo\n");



		switch (fork()) {
			case 0:
				child_process(r.larger_than_array, r.larger_than_array_len);
				break;
			case -1:
				perror("fork");
				exit(EXIT_FAILURE);
				break;
			default:
				break;
		}

		printf("[parent] aspetto che il mio secondo figlio termini...\n");
		if (wait(NULL) == -1) {
			perror("wait error");
		}
		else
			printf("[parent] terminato il secondo\n\n\n");

		printf("Ho finito il mio lavoro!\nBye");

		free(r);

		exit(EXIT_SUCCESS);

}















