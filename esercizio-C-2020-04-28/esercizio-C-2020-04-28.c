/*
scrivere il seguente programma:

verifica se esiste la cartella src; se non esiste, crea la cartella src
(creare directory: man 2 mkdir, controllo esistenza file,
ad esempio: https://github.com/marcotessarotto/exOpSys/blob/6bd8188175c5d4cc380808cbce2befebac65557b/011filesrw/01filesrw.c#L17 )

dentro src, se non esiste già, crea il file hello_world.c contenente il codice sorgente del programma che scrive "hello world" su stdout.

dentro src, crea un file output.txt; se il file esiste già, lo "tronca" a dimensione 0

il programma riceve in un suo signal handler il segnale SIGCHLD
esempio: https://github.com/marcotessarotto/exOpSys/blob/6bd8188175c5d4cc380808cbce2befebac65557b/006.3signals4ipc/signals4ipc.c#L136

entra in un ciclo senza fine:

monitora il file hello_world.c e ogni volta che viene notificato un evento di modifica del file (quando il file viene modificato da un editor quale pico, gedit o altro) il programma esegue il compilatore gcc per compilare il codice sorgente contenuto nel file hello_world.c e produrre un file eseguibile di nome hello (gcc hello_world.c -o hello).
il programma aspetta la terminazione di gcc (e riceve il valore di uscita di gcc perchè servirà nel punto successivo).

*seconda parte dell'esercizio:*
se gcc ha successo, il programma invoca fork() ed il processo figlio esegue il programma hello appena compilato dove però l'output di hello va "redirezionato" sul file output.txt (quello creato all'avvio del programma, più sopra).
il programma non aspetta la terminazione del processo figlio ma "prende nota" del PID del processo figlio aggiungendolo in coda ad un array dinamico chiamato pid_list.

gestire i processi zombie:
ogni volta che un processo figlio termina, il programma riceve un segnale SIGCHLD ed invoca wait.
vedere ad esempio: https://github.com/marcotessarotto/exOpSys/blob/6bd8188175c5d4cc380808cbce2befebac65557b/006.3signals4ipc/signals4ipc.c#L105
[il signal handler potrebbe anche rimuovere il PID del processo figlio "aspettato" da pid_list ma ci può essere un problema di concorrenza nell'accesso a pid_list, questo aspetto va discusso]

*terza parte dell'esercizio*
con un meccanismo da definire più in dettaglio (cioè: parliamone in classe), il programma termina i processi figli eseguiti nel punto precedente, se questi rimangono in esecuzione per più di 10 secondi.
[il controllo sui processi figli ancora in esecuzione potrebbe essere fatto ogni volta che il programma passa attraverso la seconda parte dell'esercizio oppure periodicamente (ad es. ogni secondo) attraverso un timer (che non abbiamo ancora visto a lezione) con l'uso di semafori (che vedremo le prossime lezioni)]

 */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <signal.h>

#include <stdbool.h>

#include <sys/inotify.h>
#include <limits.h>


const char *pathname_dir = "src";
const char *pathname_hw = "src/helloworld.c";
const char *pathname_output = "src/output.txt";

int check_file_exist(const char * fname);
off_t get_file_size(int fd);
void parent_process_signal_handler(int sig);
void inotify_event(const char * pathname);
static void show_inotify_event(struct inotify_event *i);
void create_new_child();

const char *codice_hw = "#include<stdio.h>\nint main() {\nprintf(\"hello world!\");\nreturn 0;\n}";

#define BUF_LEN 4096

char buf[BUF_LEN] __attribute__ ((aligned(__alignof__(struct inotify_event))));

pid_t *pid_list;
int pid_list_size;




int main(int argc, char *argv[]){

	if(check_file_exist(pathname_dir)==0){
		//creo la cartella src
		printf("Cartella non trovata... la sto creando...\n");
		if(mkdir(pathname_dir, S_IRUSR | S_IWUSR | S_IXUSR)==-1){
			perror("mkdir");
			exit(EXIT_FAILURE);
		}
	}

	int fd1 = open(pathname_hw, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	if (fd1 == -1) {
		perror("create_file error\n");
		return 0;
	}

	if(get_file_size(fd1)==0){
		int res = write(fd1, codice_hw, strlen(codice_hw));
		if(res==-1){
			perror("write");
			exit(EXIT_FAILURE);
		}
	}

	int fd2 = open(pathname_output, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);

	if (fd2 == -1) {
		perror("create_file error\n");
		return 0;
	}





	if(system ("gcc src/helloworld.c -o src/hello")==-1){
		perror("system");
		exit(EXIT_FAILURE);
	}


	inotify_event(pathname_hw);





	exit(EXIT_SUCCESS);
}




int check_file_exist(const char * fname) {


	if( access( fname, F_OK ) != -1 ) {
	    // file exists
		return 1;
	} else {
	    // file doesn't exist
		perror("access");
		return 0;
	}

}

off_t get_file_size(int fd){

	off_t file_offset = lseek(fd, 0, SEEK_END);

	if (file_offset == -1) {
		perror("lseek()");
		exit(EXIT_FAILURE);
	}

	printf("size of file: %ld\n", file_offset);

	return file_offset;

}


void parent_process_signal_handler(int sig){

	//un figlio è terminato

	pid_t child_pid = wait(NULL);
	printf("[parent] signal handler: processo %u è terminato\n", child_pid);

	pid_list=realloc(pid_list, (pid_list_size+1)*sizeof(pid_t));
	if(pid_list==NULL){
		perror("realloc");
		exit(EXIT_FAILURE);
	}

	pid_list[pid_list_size]=child_pid;
	pid_list_size++;

	print_list();

}

void print_list(){
	for (int i=0; i<pid_list_size; i++){
		printf("%u\n", pid_list[i]);
	}
}

void inotify_event(const char * pathname){

	int wd;
	int inotifyFd;
	int num_bytes_read;


	// inotify_init() initializes a new inotify instance and
	// returns a file descriptor associated with a new inotify event queue.
	inotifyFd = inotify_init();
	if (inotifyFd == -1) {
		perror("inotify_init");
		exit(EXIT_FAILURE);
	}

	// inotify_add_watch()  adds  a  new  watch, or modifies an existing watch,
	// for the file whose location is specified in pathname
	wd = inotify_add_watch(inotifyFd, pathname, IN_MODIFY);
	if (wd == -1) {
	    perror("inotify_init");
	    exit(EXIT_FAILURE);
	}

	// we do not keep wd...

	printf("Watching %s using wd %d\n", pathname, wd);


	printf("ready!\n\n");

	// loop forever
	for (;;) {
		num_bytes_read = read(inotifyFd, buf, BUF_LEN);
	    if (num_bytes_read == 0) {
	        printf("read() from inotify fd returned 0!");
	        exit(EXIT_FAILURE);
	    }
	    if (num_bytes_read == -1) {
		   	if (errno == EINTR) {
				printf("read(): EINTR\n");
				continue;
		    } else {
		        perror("read()");
		        exit(EXIT_FAILURE);
		    }
		}

	    printf("read %d bytes from inotify fd\n", num_bytes_read);

	    // process all of the events in buffer returned by read()

	    struct inotify_event *event;

	    for (char * p = buf; p < buf + num_bytes_read; ) {
	        event = (struct inotify_event *) p;

	        show_inotify_event(event);

	        p += sizeof(struct inotify_event) + event->len;
	        // event->len is length of (optional) file name
	    }
	}
}


static void show_inotify_event(struct inotify_event *i){

	printf("[wd=%2d] ", i->wd);

    if (i->cookie > 0)
        printf("cookie=%4d ", i->cookie);

    // The  name  field is present only when an event is returned for a file
    // inside a watched directory; it identifies the filename within the watched directory.
    // This filename is null-terminated .....

    if (i->len > 0)
        printf("file name = %s ", i->name);
    else
    	printf("*no file name* "); // event refers to watched directory

    // see man inotify
    // for explanation of events


    if (i->mask & IN_MODIFY){
    	printf("Mask: IN_MODIFY\n");
    	if(system ("gcc src/helloworld.c -o src/hello")==-1){
    		perror("system");

    	}
    	else{
    		create_new_child();
    	}
    }



}



void create_new_child(){


	char * newargv[] = {"hello", NULL};
	char * newenviron[] = { NULL };
	int fd_out;

	printf("Ora creo il figlio\n");
	switch(fork()){
		case 0:
			//child
			printf("[child]: ora avvio il programma helloworld\n");

			if((fd_out=open(pathname_output, O_WRONLY | O_APPEND))==-1){
				perror("open output.txt");
				exit(EXIT_FAILURE);
			}
			if(dup2(fd_out, STDOUT_FILENO)==-1){
				perror("dup2");
				exit(EXIT_FAILURE);
			}

			execve("./src/hello", newargv, newenviron);

			perror("execve");
			exit(EXIT_FAILURE);
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		default:
			if (signal(SIGCHLD, parent_process_signal_handler) == SIG_ERR) {
						perror("signal");
						exit(EXIT_FAILURE);
			}
		break;

		}


}






