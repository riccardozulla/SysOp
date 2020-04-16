//***convertire in codice C il seguente programma in Java***
//
//tratto da:
//https://algorithms.tutorialhorizon.com/find-the-only-element-in-array-which-appears-only-once/
//
//trova l'unico elemento dell'array che appare una volta sola
//IPOTESI: tutti gli elementi dell'array compaiono esattamente due volte, tranne un unico elemento che compare una volta sola.
//
//Esempio:
//
//int [] a = { 1,5,6,2,1,6,4,3,2,5,3};
//output: 4
//
//
//scrivere in C la versione "forza bruta" dell'algoritmo (il sito web riporta altre due versioni, facciamo questa come esercizio).
//usare cicli annidati e confrontare ciascun elemento dell'array con tutti gli altri elementi per tracciare l'elemento che è non-ripetuto.
//
//
//Time Complexity: O(n^2)
//
//public class EveryElementsRepeatedButOne {
//
//    public static void findBruteForce(int [] a){
//        boolean [] visited = new boolean[a.length];
//        for (int i = 0; i <a.length ; i++) {
//            int x = a[i];
//            if(visited[i]==false) {
//                boolean isDuplicate = false;
//                for (int j = i + 1; j < a.length; j++) {
//                    if (x == a[j]) {
//                        isDuplicate = true;
//                        visited[j] = true;
//                    }
//                }
//                if (!isDuplicate)
//                    System.out.println("Element appear only once in array - " + x);
//            }
//        }
//    }
//
//    public static void main(String[] args) {
//        int [] a = { 1,5,6,2,1,6,4,3,2,5,3};
//        findBruteForce(a);
//    }
//}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void findBruteForce(int*a, int length);


int main(){
	int a[] = { 1,5,6,2,1,6,4,3,2,5,3};
	int* p_a=&a[0];
	int length= sizeof(a)/sizeof(int);
    findBruteForce(p_a, length);

    exit(EXIT_SUCCESS);
}


void findBruteForce(int*a, int length){
	bool visited[length];
	for (int i = 0; i <length ; i++) {
	            int x = *(a+i);
	            if(visited[i]==false) {
	                bool isDuplicate = false;
	                for (int j = i + 1; j < length; j++) {
	                    if (x == *(a+j)) {
	                        isDuplicate = true;
	                        visited[j] = true;
	                    }
	                }
	                if (isDuplicate==false) printf("Element appear only once in array - %d", x);
	            }
	        }
	return;
}






