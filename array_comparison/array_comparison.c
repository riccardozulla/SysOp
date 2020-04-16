#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main(){
	long unsigned int array1[20]={12,23,34,45,56,67,78,89,90};
	long unsigned int array2[20]={12,23,34,45,56,67,78,89,90};

	long unsigned int * p_array1;
	p_array1 = array1;
	long unsigned int * p_array2 = &array2;

	int res=memcmp(p_array1, p_array2, 20*sizeof(long));

	printf("res: %d\n", res);

	exit(0);


}
