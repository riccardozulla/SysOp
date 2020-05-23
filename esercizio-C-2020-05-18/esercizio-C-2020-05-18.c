/*
 un processo crea due thread

#define LIMIT 100000
#define RESET_LIMIT 100

variabili globali:

 int counter;
 int reset;

usare mutex per regolare l'accesso concorrente alle variabili globali

thread1 e thread2 fanno questo:

   counter++;
   if (counter > LIMIT) {
      counter = counter - LIMIT;
      reset++;
     }

   if (reset > RESET_LIMIT)
     ...
     // thread termina

il main thread dopo aver avviato i due thread, aspetta la conclusione dei due thread poi termina.
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
#include <pthread.h>


#define LIMIT 100000
#define RESET_LIMIT 100

int counter;
int reset;

sem_t * mutex;

void * thread_function(void * arg){

	while(1){
		if (sem_wait(mutex) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}

		counter++;
		if (counter >= LIMIT) {
			counter = counter - LIMIT;
			reset++;
		}




		if (sem_post(mutex) == -1) {
			perror("sem_post");
			exit(EXIT_FAILURE);
		}

		if (reset >= RESET_LIMIT){
			return NULL;
		}


	}
	return NULL;

}


int main(int argc, char * argv[]) {

	pthread_t t1;
	pthread_t t2;
	void * res;

	int s;

	printf("initial value of counter=%d\treset=%d\n", counter, reset);

	mutex = malloc(sizeof(sem_t));

	s = sem_init(mutex,
					0, // 1 => il semaforo è condiviso tra processi,
					   // 0 => il semaforo è condiviso tra threads del processo
					1 // valore iniziale del semaforo (se mettiamo 0 che succede?)
				  );

	s = pthread_create(&t1, NULL, thread_function, NULL);

	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	s = pthread_create(&t2, NULL, thread_function, NULL);

	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}


	s = pthread_join(t1, &res);

	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(t2, &res);

	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	printf("final value of counter=%d\treset=%d\n", counter, reset);

	printf("bye\n");

	return 0;
}

