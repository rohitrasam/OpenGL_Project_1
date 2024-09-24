#include <stdio.h>
#include <stdlib.h>

void magic_square(int n){

	int zero = 0;
	if(n*n > 10 && n*n < 100){
		zero = 2;
	}
	else if(n*n >= 100 && n*n <1000){
		zero = 3;
	}
	else if(n*n >= 1000){
		zero = 4;
	}
	int **sqr = malloc(n*sizeof(int*));
	for(int i=0; i < n; i++){
		sqr[i] = malloc(n*sizeof(int));
	}

	for(int i=0; i < n; i++){
		for(int j=0; j<n; j++){
			*(*(sqr+i)+j) = 0;
		}
	}

	if(n % 2 == 0){
		printf("size is even\n");
		return;
	}
	
	sqr[0][(n-1)/2] = 1;

	int row = 0;
	int col = (n-1)/2;
	for(int key=2; key <= n*n; key++){
		if(row >= 1){
			row -= 1;
		}
		else{
			row = n-1;
		}
		if(col >= 1){
			col -= 1;
		}
		else{
			col = n-1;
		}
		if(*(*(sqr+row)+col) > 0){
			row = (row+2)%n;
			col = (col+1)%n;
		}
		*(*(sqr+row)+col) = key;
	}


	for(int i=0; i < n; i++){
		printf("[");
		for(int j=0; j<n; j++){
			if(j == n-1){
				printf("%0*d", zero, *(*(sqr+i)+j));
			}
			else{
				printf("%0*d, ", zero, *(*(sqr+i)+j));
			}
		}
		printf("],\n");
	}

}


int main()
{
	int size;
	printf("Enter size of magic square: ");
	scanf("%d", &size);
	magic_square(size);
	return 0;
}