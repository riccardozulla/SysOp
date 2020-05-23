/*
 un processo apre un file in scrittura (se esiste già sovrascrive i contenuti del file),
 	 poi lancia M (=10) threads.

"fase 1" vuol dire: dormire per un intervallo random di tempo compreso tra 0 e 3 secondi,
	poi scrivere nel file il messaggio: "fase 1, thread id=, sleep period= secondi"

"fase 2" vuol dire: scrivere nel file il messaggio "fase 2, thread id=, dopo la barriera"
	poi dormire per 10 millisecondi, scrivere nel file il messggio "thread id= bye!".

per ogni thread: effettuare "fase 1", poi aspettare che tutti i thread abbiano completato
	la fase 1 (barriera: little book of semaphores, pag. 29);
	poi effettuare "fase 2" e terminare il thread.
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
#include <pthread.h>
#include <time.h>
#include <math.h>

#define N 10

#define A 0 		//intervallo tempo "fase 1"
#define B 3000		//il random-time sarà compreso tra [A,B]

pthread_barrier_t thread_barrier;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int number_of_threads = N;
int fd;

void * thread_function(void * arg) {
	int s;

	//aspetto un tempo random di 0-3sec
	long ns; // Milliseconds
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);


	ns = round(spec.tv_nsec);
	srand(ns);
	int random_time = rand() % B + A;

	sleep(random_time/1000);


	//scrivo nel file la stringa: "fase 1, thread id=, sleep period= secondi"
	s=pthread_mutex_lock(&mutex);
	if(s!=0){
		perror("pthread_mutex_lock");
		exit(EXIT_FAILURE);
	}

	s= dprintf(fd, "fase 1, thread id = %lu, sleep period = %f\n", pthread_self(), (float)random_time/1000);
	if(s==-1){
		perror("dprintf");
		exit(EXIT_FAILURE);
	}

	s=pthread_mutex_unlock(&mutex);
	if(s!=0){
		perror("pthread_mutex_unlock");
		exit(EXIT_FAILURE);
	}


	s = pthread_barrier_wait(&thread_barrier);

	if (s != 0 && s!=PTHREAD_BARRIER_SERIAL_THREAD) {
		perror("pthread_barrier_wait");
		exit(EXIT_FAILURE);
	}


	//scrivo nel file la stringa: "fase 2, thread id=, dopo la barriera"

	s=pthread_mutex_lock(&mutex);
	if(s!=0){
		perror("pthread_mutex_lock");
		exit(EXIT_FAILURE);
	}

	s= dprintf(fd, "fase 2, thread id = %lu, dopo la barriera\n", pthread_self());
	if(s==-1){
		perror("dprintf");
		exit(EXIT_FAILURE);
	}

	s=pthread_mutex_unlock(&mutex);
	if(s!=0){
		perror("pthread_mutex_unlock");
		exit(EXIT_FAILURE);
	}


	sleep(0.01);



	s=pthread_mutex_lock(&mutex);
	if(s!=0){
		perror("pthread_mutex_lock");
		exit(EXIT_FAILURE);
	}

	s= dprintf(fd, "thread id = %lu, Bye\n", pthread_self());
	if(s==-1){
		perror("dprintf");
		exit(EXIT_FAILURE);
	}

	s=pthread_mutex_unlock(&mutex);
	if(s!=0){
		perror("pthread_mutex_unlock");
		exit(EXIT_FAILURE);
	}


	return NULL;
}




#define CHECK_ERR(a,msg) {if ((a) == -1) { perror((msg)); exit(EXIT_FAILURE); } }

int main() {

	int s;
	pthread_t threads[N];

	s = pthread_barrier_init(&thread_barrier, NULL, N);
	CHECK_ERR(s,"pthread_barrier_init")

	fd = open("output.txt", O_CREAT | O_TRUNC | O_RDWR,
				  S_IRUSR | S_IWUSR );
	CHECK_ERR(fd,"open");

	for (int i=0; i < number_of_threads; i++) {
		s = pthread_create(&threads[i], NULL, thread_function, NULL);

		if (s != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	for (int i=0; i < number_of_threads; i++) {
		s = pthread_join(threads[i], NULL);

		if (s != 0) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}

	}

	s = pthread_barrier_destroy(&thread_barrier);
	CHECK_ERR(s,"pthread_barrier_destroy")

	printf("Bye\n");

	return 0;
}

