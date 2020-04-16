//riprendiamo l'esercizio esercizio-C2020-04-03
//https://github.com/marcotessarotto/esercizio-C-2020-04-03
//
//***
//è dato il tipo dati contact_type, dovrebbe servire ad implementare una rubrica telefonica molto semplice:
//
//#define NAME_LEN 64
//#define PHONE_LEN 20
//
//typedef struct {
//   int id; // id numerico del contatto (valore univoco)
//   char name[NAME_LEN + 1]; // nome della persona
//   char phone[PHONE_LEN + 1]; // numero di telefono
//} contact_type;
//
//sono già date le seguenti funzioni (riutilizzare quelle già sviluppate):
//
//// crea un oggetto di tipo contact_type e lo inizializza con name e phone
//contact_type * create_contact(char * name, char * phone);
//
//// scrive su stdout i contenuti dell'oggetto
//void print_contact(contact_type * person);
//
//******
//
//scrivere le seguenti nuove funzioni e variabili:
//
//#define ADDRESS_BOOK_SIZE 256
//
//// la rubrica telefonica contiene al massimo 256 nominativi
//contact_type * address_book[ADDRESS_BOOK_SIZE]; // global variable
//// è un array di puntatori a contact_type
//// se address_book[i] == NULL, vuole dire che la cella è utilizzabile per conservare l'indirizzo di un contatto
//
//
//// confronta due contatti:
//// due contatti sono uguali se e solo se c1->name è uguale a c2->name e c1->phone è uguale a c2->phone
//// return value:
//// 0: contatti uguali
//// < 0: se *c1 è ordinato prima di *c2
//// > 0: se *c1 è ordinato dopo di *c2
//// ovvero FATE COME FA STRCMP (vedete man strcmp)
//// NOTA BENE: se c1 == NULL or c2 == NULL, restituisce -1
//int compare_contacts(contact_type * c1, contact_type * c2);
//
//
//// aggiunge il contatto c all'array address_book array
//// la funzione NON controlla se il contatto è già presente; ma cerca la prima cella libero nell'array
//// return value:
//// >=0: posizione nell'array address_book dove è stato inserito c
//// -1: errore (i.e. non c'è più spazio in address_book o c == NULL)
//int add_to_address_book(contact_type * c);
//
//// rimuovi il contatto c da address_book (setta a NULL la posizione dove si trova c)
//// return value:
//// >= 0: posizione dove si trovava c
//// -1: non ho trovato c, quindi non ho cancellato nessun contatto
//int remove_from_address_book(contact_type * c);
//
//
//// cerca il contatto c nell'array address_book
//// cercare un contatto: confronta c con ogni contatto memorizzato in address_book
//// DOVETE USARE compare_contacts()
//// return value:
//// >= 0: posizione dove si trova il primo contatto uguale a c
//// -1: contatto c non trovato
//int search_in_address_book(contact_type * c);
//
//
//// funzione che fa il sort dei contatti in address_book, utilizzando compare_contacts()
//void sort_by_name();
//
//
//scrivete un main() faccia le cose seguenti:
//
//1 - creare gli oggetti di tipo contact_type a partire dai seguenti nominativi e numeri di telefono
//ed inserirli in address_book utilizzando la funzione add_to_address_book
//
//creare ed inserire in rubrica i contatti in questo ordine:
//
//"dino","+391237"
//"filippo", "+391239"
//"barbara","+391235"
//"antonio","+391234"
//"enrico","+391238"
//"chiara","+391236"
//
//2 - cercare il contatto "antonio","+391234"
//e se trovato, rimuoverlo
//
//3 - creare ed aggiungere il contatto "pino","+399999"
//e scrivere a video tutta la rubrica telefonica (non le celle vuote)
//
//4 - aggiungere di nuovo dil contatto "antonio","+391234"
//
//5 - fare il sort di tutta la rubrica
//
//6 - scrivere a video tutta la rubrica telefonica (non le celle vuote)
//il risultato deve essere ordinato
//
//FINE





#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NAME_LEN 64
#define PHONE_LEN 20
#define ADDRESS_BOOK_SIZE 256

typedef struct {
   int id; // id numerico del contatto (valore univoco)
   char name[NAME_LEN + 1]; // nome della persona
   char phone[PHONE_LEN + 1]; // numero di telefono
} contact_type;


unsigned int ID;
contact_type * address_book[ADDRESS_BOOK_SIZE];



contact_type * create_contact(char * name, char * phone);
void print_contact(contact_type * person);
int compare_contacts(contact_type * c1, contact_type * c2);
int add_to_address_book(contact_type * c);
int remove_from_address_book(contact_type * c);
int search_in_address_book(contact_type * c);
void sort_by_name();
void swap(contact_type * a, contact_type * b);

void print_address_book();










contact_type * create_contact(char * name, char * phone){

	contact_type *newContact;
	newContact = calloc(1, sizeof(contact_type));


	newContact->id=ID++;

	if (name != NULL)
			strncpy(newContact->name, name, NAME_LEN);

	if (phone != NULL)
			strncpy(newContact->phone, phone, PHONE_LEN);

	return newContact;

}

void print_contact(contact_type * person){

	printf("ID: %d\tNAME: %s\tPHONE: %s\t\n", person->id, person->name, person->phone);



}


int compare_contacts(contact_type * c1, contact_type * c2){
	if (c1 == NULL || c2 == NULL) return -1;
	int name=strcmp(c1->name, c2->name);
	int phone=strcmp(c1->phone,c2->phone);
	if (name==0&&phone==0)return 0;
	else return name;
}

int add_to_address_book(contact_type * c){
	if(c==NULL) return -1;
	int position = 0;
	while(address_book[position]!=NULL && position<ADDRESS_BOOK_SIZE){
		position++;
	}

	if(position==ADDRESS_BOOK_SIZE){
		return -1;
	}
	else{
		address_book[position]=c;
		return position;
	}

}

int remove_from_address_book(contact_type * c){
	int r=search_in_address_book(c);
	if(r!=-1)
		address_book[r]=NULL;
	return r;
}


int search_in_address_book(contact_type * c){
	for(int i=0; i<ADDRESS_BOOK_SIZE; i++){
		if(compare_contacts(c, address_book[i])==0) return i;
	}
	return -1;
}


void sort_by_name(){
	int n=ADDRESS_BOOK_SIZE;

	while(n>1){
		int newn=0;

		for(int i=1; i<n; i++){
			if(compare_contacts(address_book[i-1], address_book[i])>0){
				swap(address_book[i-1],address_book[i]);
				newn=i;
			}
		}
		n=newn;
	}
}




void swap(contact_type * a, contact_type * b){
	contact_type mem = *a;
	*a=*b;
	*b=mem;
}




//"dino","+391237"
//"filippo", "+391239"
//"barbara","+391235"
//"antonio","+391234"
//"enrico","+391238"
//"chiara","+391236"
int main(int argc, char *argv[]){

	//punto 1 - creazione contatti ed inserimento in address_book
	contact_type * dino= create_contact("dino", "+391237");

		if (dino == NULL) {
			printf("errore in create_contact!\n");
			exit(EXIT_FAILURE);
		}
		if(add_to_address_book(dino)==-1){
			printf("errore in add_to_address_book!\n");
			exit(EXIT_FAILURE);
		}

	contact_type * filippo= create_contact("filippo", "+391239");

		if (filippo == NULL) {
			printf("errore in create_contact!\n");
			exit(EXIT_FAILURE);
		}
		if(add_to_address_book(filippo)==-1){
			printf("errore in add_to_address_book!\n");
			exit(EXIT_FAILURE);
		}

	contact_type * barbara= create_contact("barbara","+391235");

		if (barbara == NULL) {
			printf("errore in create_contact!\n");
			exit(EXIT_FAILURE);
		}
		if(add_to_address_book(barbara)==-1){
			printf("errore in add_to_address_book!\n");
			exit(EXIT_FAILURE);
		}

	contact_type * antonio= create_contact("antonio","+391234");

		if (antonio == NULL) {
			printf("errore in create_contact!\n");
			exit(EXIT_FAILURE);
		}
		if(add_to_address_book(antonio)==-1){
			printf("errore in add_to_address_book!\n");
			exit(EXIT_FAILURE);
		}

	contact_type * enrico = create_contact("enrico","+391238");

		if (enrico == NULL) {
			printf("errore in create_contact!\n");
			exit(EXIT_FAILURE);
		}
		if(add_to_address_book(enrico)==-1){
			printf("errore in add_to_address_book!\n");
			exit(EXIT_FAILURE);
		}


	contact_type * chiara = create_contact("chiara","+391236");

		if (chiara == NULL) {
			printf("errore in create_contact!\n");
			exit(EXIT_FAILURE);
		}
		if(add_to_address_book(chiara)==-1){
			printf("errore in add_to_address_book!\n");
			exit(EXIT_FAILURE);
		}




		//punto 2 - trovare e rimuovere il contatto antonio

		if(remove_from_address_book(antonio)!=-1)
			printf("Ho rimosso il contatto Antonio con successo!\n");
		else
			printf("errore in remove_from_address_book(antonio) o in \n");


		//punto 3 - creare il contatto pino e stampare la rubrica

		contact_type * pino = create_contact("pino","+399999");

			if (pino == NULL) {
				printf("errore in create_contact!\n");
				exit(EXIT_FAILURE);
			}
			if(add_to_address_book(pino)==-1){
				printf("errore in add_to_address_book!\n");
				exit(EXIT_FAILURE);
			}

			print_address_book();

			//punto 4 - riaggiungere il contatto antonio

			if(add_to_address_book(antonio)==-1){
				printf("errore in add_to_address_book!\n");
				exit(EXIT_FAILURE);
			}
			print_address_book();




			//punto 5 - fare il sortName della rubrica e stampare tutto

			sort_by_name();
			print_address_book();


			//elimino spazio su memoria
			free(dino);
			free(antonio);
			free(filippo);
			free(barbara);
			free(enrico);
			free(chiara);
			free(pino);




return 0;
}




void print_address_book(){
	printf("Stampo tutta la rubrica: \n");
	for(int i=0; i<ADDRESS_BOOK_SIZE; i++){
		if(address_book[i]!=NULL) print_contact(address_book[i]);
	}
}










