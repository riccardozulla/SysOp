#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * string_rand(char * string, int length);
char * token_rand(char ** array_token, int length);

int main() {

	char  string[]="hello world mi chiamo pino sono date delle stringhe";
	int length = sizeof(string)/sizeof(char);

	char * rand = string_rand(&string[0], length);

	printf("la stringa è: %s", rand);



	exit(0);
}


char * string_rand(char * string, int length){

	char s[] = " "; // caratteri separatori da utilizzare per la suddivisione
	char** token;

	token = calloc(1, sizeof(char));

	// ottieni il primo token
	int t=0;
	token[t] = strtok(string, s);


	// "cammina" attraverso gli altri token
	while (token[t] != NULL ) {
		printf( "%s\n", token[t] );
		t++;
		token=realloc(token, sizeof(token)*(t+1));
		token[t] = strtok(NULL, s); // le chiamate successive a strtok

	}

	//return token[4];
	return token_rand(token, t);


}





char * token_rand(char ** array_token, int length){

	char * stringa;
	stringa= malloc(1*sizeof(char));
	stringa[0] ='\0';
	for(int i =0; i<length ; i++){
		int casuale;

		do{
			casuale=rand()%(length);
		}while(array_token[casuale]==NULL);

			int new_len= strlen(array_token[casuale]);
			stringa =realloc(stringa, (strlen(stringa)+new_len)*sizeof(char));
			if(stringa==NULL){
				perror("realloc ha fallito");
				exit(EXIT_FAILURE);
			}


			//strcat stringa, " ");
			strcat(stringa, array_token[casuale]);
			strcat(stringa, " ");
			array_token[casuale]=NULL;
	}
	free(array_token);
	return stringa;

}
