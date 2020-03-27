

//BUBBLESORT

/*
procedure bubbleSort(A : list of sortable items)
    n := length(A)
    repeat
        newn := 0
        for i := 1 to n - 1 inclusive do
            if A[i - 1] > A[i] then
                swap(A[i - 1], A[i])
                newn := i
            end if
        end for
        n := newn
    until n ≤ 1
end procedure
*/

#include<stdio.h>;

void bubble_sort(int*array, int length);
void swap_int(int * a, int * b);

void main (){


	int array [] = {10,12,32,1,43,99,13,54,32,86,46,75,61,12};
	int size=sizeof(array)/sizeof(int);

	int*p_array = &array;

	bubble_sort(p_array, size);

	print_array(p_array, size);

}




void bubble_sort(int*array, int length){
	int n=length;
	while(n>1){
		int newn=0;

		for(int i=1; i<n; i++){
			if(array[i-1]>array[i]){
				swap_int(&array[i-1],&array[i]);
				newn=i;
			}
		}
		n=newn;
	}
}




void swap_int(int * a, int * b){
	int mem=*a;
	*a=*b;
	*b=mem;
}



void print_array(int * array, int length){
	printf("L'array è definito da:\n ");
	for(int i=0; i<length; i++){
		printf("array[%u]\t%u\n", i, array[i]);
	}
}
















