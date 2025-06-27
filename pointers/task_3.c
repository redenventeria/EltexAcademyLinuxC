
#include<stdio.h>
#include<stdlib.h>

#define BUF_SIZE 30

int main()
{
	const size_t N = 10;
	int a[] = {3, 5, 4, 2, 6, 1, 6, 4, 5, 1};
	
	for(size_t i = 0; i < N; i++)
	{
		printf("%d ", *(a + i));
	}
	printf("\n");

	return 0;
}


