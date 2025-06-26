#include<stdlib.h>
#include<stdio.h>

#define MAX_ARRAY_SIZE 50


int main()
{
	int n = 4;
	int m[4][4] = {{0}};

	
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			m[i][j] = (i + j + 1 < n) ? 0 : 1;

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++)
			printf("%d ", m[i][j]);
		printf("\n");
	}

		
}
