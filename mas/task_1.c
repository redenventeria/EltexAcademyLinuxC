#include<stdio.h>
#include<stdlib.h>


void print_matrix(unsigned int n)
{
	if(n == 0) {
		printf("Error, n must be non-zero!");
		return;
	}


	int **m = malloc(sizeof(int*) * n);
	
	for(size_t i = 0; i < n; i++)
		m[i] = malloc(sizeof(int) * n);
	
	int num = 1;
	for(size_t i = 0; i < n; i++)
		for(size_t j = 0; j < n; j++) {
			m[i][j] = num;
			num++;
		}	


	for(size_t i = 0; i < n; i++) {
		for(size_t j = 0; j < n; j++)
			printf("%d ", m[i][j]);
		free(m[i]);
		printf("\n");
	}
	
	free(m);

}


int main()
{
	printf("N = 3\n");
	print_matrix(3);

	printf("N = 5\n");
	print_matrix(5);
	return 0;
}
