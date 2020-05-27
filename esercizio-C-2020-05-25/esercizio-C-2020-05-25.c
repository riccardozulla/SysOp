/*
il processo principale crea un file "output.txt" di dimensione FILE_SIZE
(all'inizio ogni byte del file deve avere valore 0)

#define FILE_SIZE (1024*1024)

#define N 4

è dato un semaforo senza nome: proc_sem

il processo principale crea N processi figli

i processi figli aspettano al semaforo proc_sem.

ogni volta che il processo i-mo riceve semaforo "verde", cerca il primo byte del file che abbia valore 0
e ci scrive il valore ('A' + i). La scrittura su file è concorrente e
quindi va gestita opportunamente (ad es. con un mutex).

se il processo i-mo non trova una posizione in cui poter scrivere il valore, allora termina.

il processo padre:

per (FILE_SIZE+N) volte, incrementa il semaforo proc_sem

aspetta i processi figli e poi termina.

risolvere il problema in due modi:

soluzione A:

usare le system call open(), lseek(), write()

soluzione B:

usare le system call open(), mmap()


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



#define FILE_SIZE (10)	//1024*1024
#define N 2				//4

#define CHECK_ERR(a,msg) {if ((a) == -1) { perror((msg)); exit(EXIT_FAILURE); } }
#define CHECK_ERR_MMAP(a,msg) {if ((a) == MAP_FAILED) { perror((msg)); exit(EXIT_FAILURE); } }


const char *output_A = "outputA.txt";
const char *output_B = "outputB.txt";

char * data;

sem_t * proc_sem;
sem_t * mutex;

int cont;

//#define DEBUG_MSG


void child_functionA(int i){


	int s;
	int exit_while=1;
	char buf;
	char ch='A'+i;

	int fd = open(output_A,  O_RDWR);
	CHECK_ERR(fd, "open")

	while(exit_while){

		if (sem_wait(proc_sem) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}

		//semaforo "verde"


		if (sem_wait(mutex) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}

		int res;
		while((res=read(fd, &buf, sizeof(char)))>0 && buf!=0)
			continue;

		if(res==1){		//abbiamo trovato una posizione libera
			s=lseek(fd, -1, SEEK_CUR);
			CHECK_ERR(s, "lseek")

#ifdef DEBUG_MSG
			printf("[child %d] offset = %d\n", i, s);
#endif

			s=write(fd, &ch, sizeof(ch));
			CHECK_ERR(s, "write")

			cont++;

		}
		else{		//siamo alla fine del file
			printf("[child %d] file terminato; cont = %d\n", i, cont);
			exit_while=0;
		}




		if (sem_post(mutex) == -1) {
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
	}

	exit(EXIT_SUCCESS);

}



void soluzione_A(){

	int s;

	int fd = open(output_A, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR );
	CHECK_ERR(fd, "open")

	s = ftruncate(fd, FILE_SIZE);
	CHECK_ERR(s, "ftruncate")

	close(fd);

	proc_sem = mmap(NULL, sizeof(sem_t)*2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	CHECK_ERR_MMAP(proc_sem,"mmap")

	mutex=proc_sem+1;

	s = sem_init(proc_sem, 1, 0 );
	CHECK_ERR(s,"sem_init: proc_sem")

	s = sem_init(mutex, 1, 1 );
	CHECK_ERR(s,"sem_init: mutex")


	//creaiamo N processi figli
	for(int i=0; i<N; i++){
		switch(fork()){
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0: //child
			child_functionA(i);
			break;
		}
	}

	sleep(1);
	for(int i=0; i<FILE_SIZE+N; i++){
		if (sem_post(proc_sem) == -1) {
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
	}

	while(wait(NULL)!=-1)
		continue;

	if(errno!=ECHILD){
		perror("wait");
		exit(EXIT_FAILURE);
	}

	s = sem_destroy(proc_sem);
	CHECK_ERR(s,"sem_destroy")


}



void child_functionB(int i){

	int exit_while=1;
	char ch='A'+i;
	char *ptr_file;
	ptr_file=data;



	while(exit_while){


		if (sem_wait(proc_sem) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}

		//semaforo "verde"


		if (sem_wait(mutex) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}

		while(*ptr_file!=0 && ptr_file-data<FILE_SIZE)
			ptr_file++;

		if(ptr_file-data==FILE_SIZE){
			printf("[child %d] file terminato; cont = %d\n", i, cont);
			exit_while=0;
		}
		else{ 		//trovata posizione in cui scrivere

#ifdef DEBUG_MSG
			printf("[child %d] puntatore = %p\toffset = %ld\n", i, ptr_file, ptr_file-data);
#endif

			*ptr_file=ch;
#ifdef DEBUG_MSG
			printf("ptr_file = %c\n", *ptr_file);
			printf("tot = %s\n", data);
#endif
			ptr_file++;
			cont++;

		}


		if (sem_post(mutex) == -1) {
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}



void soluzione_B(){
	int s;

		int fd = open(output_B, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR );
		CHECK_ERR(fd, "open")

		s = ftruncate(fd, FILE_SIZE);
		CHECK_ERR(s, "ftruncate")


		data = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		CHECK_ERR_MMAP(data,"mmap: data")

		//close(fd);



		proc_sem = mmap(NULL, sizeof(sem_t)*2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		CHECK_ERR_MMAP(proc_sem,"mmap: proc_sem")

		mutex=proc_sem+1;

		s = sem_init(proc_sem, 1, 0 );
		CHECK_ERR(s,"sem_init: proc_sem")

		s = sem_init(mutex, 1, 1 );
		CHECK_ERR(s,"sem_init: mutex")


		//creaiamo N processi figli
		for(int i=0; i<N; i++){
			switch(fork()){
			case -1:
				perror("fork");
				exit(EXIT_FAILURE);
			case 0: //child
				child_functionB(i);
				break;
			}
		}

		sleep(1);
		for(int i=0; i<FILE_SIZE+N; i++){
			if (sem_post(proc_sem) == -1) {
				perror("sem_post");
				exit(EXIT_FAILURE);
			}
		}

		while(wait(NULL)!=-1)
			continue;

		if(errno!=ECHILD){
			perror("wait");
			exit(EXIT_FAILURE);
		}

		s = sem_destroy(proc_sem);
		CHECK_ERR(s,"sem_destroy")

}


int main() {
    printf("ora avvio la soluzione_A()...\n");
    soluzione_A();

    sleep(1);
    printf("ed ora avvio la soluzione_B()...\n");
    soluzione_B();

    printf("bye!\n");
    return 0;
}


