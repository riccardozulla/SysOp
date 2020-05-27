/*
 il processo padre per comunicare con il processo figlio prepara:
- una pipe
- una memory map condivisa

il processo padre manda i dati al processo figlio attraverso la pipe

il processo figlio restituisce il risultato attraverso la memory map convidisa

il processo padre prende come argomento a linea di comando un nome di file.
il processo padre legge il file e manda i contenuti attraverso la pipe al processo figlio.

il processo figlio riceve attraverso la pipe i contenuti del file e calcola SHA3_512.

quando la pipe raggiunge EOF, il processo figlio produce il digest di SHA3_512
e lo scrive nella memory map condivisa, poi il processo figlio termina.

quando al processo padre viene notificato che il processo figlio ha terminato,
prende il digest dalla memory map condivisa e lo scrive a video
("SHA3_512 del file %s è il seguente: " <segue digest in formato esadecimale>).
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

#include <openssl/evp.h>


#define FILE_SIZE 512
unsigned int file_size=FILE_SIZE;

#define BUF_SIZE 1024*1024*512

void child_process(int pfd[2]);
unsigned char * sha3_512(char * addr, unsigned int size, int * result_len_ptr);



char * pathname;
char *addr;

int main(int argc, char *argv[]){
	if (argc!=2){
		printf("Inserire un file da processare\n");
		pathname="/home/utente/prova.txt";
	}
	else pathname=argv[1];

	//apriamo il file con il testo da elaborare
	int fd=open(pathname,O_RDWR);
	if(fd==-1){
		perror("open");
		exit(EXIT_FAILURE);
	}

	//creiamo la pipe
	int pfd[2];
	int res=pipe(pfd);
	if (res==-1){
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	//creiamo il mapping
	addr=mmap(NULL, file_size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

	if(addr==MAP_FAILED){
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	memset(addr, 0xFF, file_size);




	//fork
	switch(fork()){
	case 0:
		child_process(pfd);
		exit(EXIT_SUCCESS);
	case -1:
		perror("fork");
		exit(EXIT_FAILURE);
	}

	close(pfd[0]);

	/*if(dup2(pfd[1], fd)==-1){
		perror("dup2");
		exit(EXIT_FAILURE);
	}*/

	char * msg=calloc(BUF_SIZE, sizeof(char));

	read(fd, msg, BUF_SIZE);
	printf("Il messaggio letto: \n%s\n", msg);
	write(pfd[1], msg, strlen(msg));
	close(pfd[1]);

	pid_t child_pid = wait(NULL);
	printf("Il figlio PID:%u è terminato! \n", child_pid);
	printf("Il risultato SHA3 512: \n");

	for (int i=0; i<512/8; i++){
		printf("%02x",addr[i]&0xFF);
	}
	printf("\nBYE BYE\n");

	exit(EXIT_SUCCESS);


}


void child_process(int pfd[2]){
	int digest_len;
	unsigned char *digest;
	char *buf = calloc(BUF_SIZE, sizeof(char));
	if(buf==NULL){
		perror("calloc");
		exit(EXIT_FAILURE);
	}


	close(pfd[1]);

	while (1) {

		int numRead = read(pfd[0], buf, BUF_SIZE);
		if (numRead == -1) {
			perror("errore in read");
			exit(EXIT_FAILURE);
		}

		if (numRead == 0)
			break; // EOF: la pipe è stata chiusa dal lato di scrittura
	}

	printf("La stringa trasmessa attraverso la pipe ha %ld caratteri ed è: \n%s\n", strlen(buf), buf);
	close(pfd[0]);
	digest =sha3_512(buf, strlen(buf), &digest_len);

	printf("[child] SHA3_512 on %u bytes:\n", file_size);
	for (int i = 0; i < digest_len; i++) {
		printf("%02x", digest[i]);
	}
	printf("\n");

	memcpy(addr, digest, digest_len);


}

#define HANDLE_ERROR(msg) { fprintf(stderr, "%s\n", msg); exit(EXIT_FAILURE); }
#define HANDLE_ERROR2(msg, mdctx) { fprintf(stderr, "%s\n", msg); EVP_MD_CTX_destroy(mdctx); exit(EXIT_FAILURE); }

unsigned char * sha3_512(char * addr, unsigned int size, int * result_len_ptr) {

	EVP_MD_CTX * mdctx;
	int val;
	unsigned char * digest;
	unsigned int digest_len;
	EVP_MD * algo = NULL;

	algo = EVP_sha3_512();

	if ((mdctx = EVP_MD_CTX_create()) == NULL) {
		HANDLE_ERROR("EVP_MD_CTX_create() error")
	}

	// initialize digest engine
	if (EVP_DigestInit_ex(mdctx, algo, NULL) != 1) { // returns 1 if successful
		HANDLE_ERROR2("EVP_DigestInit_ex() error", mdctx)
	}

	// provide data to digest engine
	if (EVP_DigestUpdate(mdctx, addr, size) != 1) { // returns 1 if successful
		HANDLE_ERROR2("EVP_DigestUpdate() error", mdctx)
	}

	digest_len = EVP_MD_size(algo); // sha3_512 returns a 512 bit hash

	if ((digest = (unsigned char *)OPENSSL_malloc(digest_len)) == NULL) {
		HANDLE_ERROR2("OPENSSL_malloc() error", mdctx)
	}

	// produce digest
	if (EVP_DigestFinal_ex(mdctx, digest, &digest_len) != 1) { // returns 1 if successful
		OPENSSL_free(digest);
		HANDLE_ERROR2("EVP_DigestFinal_ex() error", mdctx)
	}

	char * result = malloc(digest_len);
	if (result == NULL) {
		perror("malloc()");
		exit(EXIT_FAILURE);
	}

	memcpy(result, digest, digest_len);

	*result_len_ptr = digest_len;

	OPENSSL_free(digest);
	EVP_MD_CTX_destroy(mdctx);

	return result;
}


