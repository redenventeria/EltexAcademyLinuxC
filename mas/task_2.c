#include<stdio.h>
#include<stdlib.h>

void print_reversed(int *m, size_t N)
{
	for(size_t i = N; i --> 0;) // reverse loop has range of [N - 1; 0]
		printf("%d ", m[i]);
	printf("\n");
	return;
}

int main()
{
	int N1 = 5;
	int m1[] = {3, 7, 1, 6, 2};
	print_reversed(m1, N1);


	int N2 = 12;
	int m2[] = {35, -24, -54, 66, 88, 12, 43, 67, 23, -1, 11, 13};
	print_reversed(m2, N2);


	int N3 = 7;
	int m3[] = {3, 7, 1, 6, 2, 4, 6};
	print_reversed(m3, N3);
}
