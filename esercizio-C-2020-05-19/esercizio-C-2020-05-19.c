/*
 N = 10

un processo padre crea N processi figli ( https://repl.it/@MarcoTessarotto/crea-n-processi-figli )

shared variables: countdown, process_counter[N], shutdown

usare mutex per regolare l'accesso concorrente a countdown

dopo avere avviato i processi figli, il processo padre dorme 1 secondo e poi imposta il valore di countdown al valore 100000.

quando countdown == 0, il processo padre imposta shutdown a 1.

aspetta che terminino tutti i processi figli e poi stampa su stdout process_counter[].

i processi figli "monitorano" continuamente countdown:

    processo i-mo: se countdown > 0, allora decrementa countdown ed incrementa process_counter[i]

    se shutdown != 0, processo i-mo termina

 */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <errno.h>
#include <semaphore.h>


#define N 10

int *countdown;
int *process_counter;
int *shutdown;


sem_t * process_semaphore;

void child_function(int i){
	while(1){
		if (sem_wait(process_semaphore) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}

		if(*countdown!=0){
			(*countdown)--;
			process_counter[i]++;
		}



		if (sem_post(process_semaphore) == -1) {
			perror("sem_post");
			exit(EXIT_FAILURE);
		}

		if(*shutdown==1)
			exit(EXIT_SUCCESS);
	}
}

int main (int argc, char *argv[]){


	int res;

	process_semaphore = mmap(NULL, // NULL: è il kernel a scegliere l'indirizzo
				sizeof(sem_t) + sizeof(int), // dimensione della memory map
				PROT_READ | PROT_WRITE, // memory map leggibile e scrivibile
				MAP_SHARED | MAP_ANONYMOUS, // memory map condivisibile con altri processi e senza file di appoggio
				-1,
				0); // offset nel file

	res = sem_init(process_semaphore,
						1, // 1 => il semaforo è condiviso tra processi, 0 => il semaforo è condiviso tra threads del processo
						1 // valore iniziale del semaforo (se mettiamo 0 che succede?)
					  );

	if(res==-1){
		perror("sem_int");
		exit(EXIT_FAILURE);
	}


	countdown = mmap(NULL, // NULL: è il kernel a scegliere l'indirizzo
			sizeof(int), // dimensione della memory map
			PROT_READ | PROT_WRITE, // memory map leggibile e scrivibile
			MAP_SHARED | MAP_ANONYMOUS, // memory map condivisibile con altri processi
			-1,
			0); // offset nel file


	if (countdown == MAP_FAILED) {
		perror("mmap()");
		return 1;
	}

	shutdown = mmap(NULL, // NULL: è il kernel a scegliere l'indirizzo
			sizeof(int), // dimensione della memory map
			PROT_READ | PROT_WRITE, // memory map leggibile e scrivibile
			MAP_SHARED | MAP_ANONYMOUS, // memory map condivisibile con altri processi
			-1,
			0); // offset nel file


	if (shutdown == MAP_FAILED) {
		perror("mmap()");
		return 1;
	}
	process_counter = mmap(NULL, // NULL: è il kernel a scegliere l'indirizzo
				sizeof(int)*N, // dimensione della memory map
				PROT_READ | PROT_WRITE, // memory map leggibile e scrivibile
				MAP_SHARED | MAP_ANONYMOUS, // memory map condivisibile con altri processi
				-1,
				0); // offset nel file


		if (process_counter == MAP_FAILED) {
			perror("mmap()");
			return 1;
		}


	printf("[parent] creo i figli\n");
	for(int i=0; i<N; i++){

		switch(fork()){
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);

		case 0:
			child_function(i);
			break;
		default:
			break;
		}
	}

	sleep(1);
	*countdown=100000;

	printf("[parent] aspetto decrementino countdown\n");
	while(*countdown!=0)
		continue;

	*shutdown=1;
	printf("[parent] countdown = %d\tshutdown = %d ......aspetto terminino i figli\n", *countdown, *shutdown);

	int n=0;
	while(n<N){
		wait(NULL);
		n++;
		printf("figli=%d\n",n );
	}

	int sum=0;
	for(int i=0; i<N; i++){
		printf("process_counter[%d]=%d\n", i, process_counter[i]);
		sum=sum+process_counter[i];
	}
	printf("totale = %d\n", sum);

	printf("bye");
	exit(EXIT_SUCCESS);


}



