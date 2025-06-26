#include<stdio.h>
#include<stdlib.h>

// 1. Вывести двоичное представление целого положительного числа, используя битовые операции (число вводится с клавиатуры).

#define INT_SIZE 32

int merge_bytes(int a, int b)
{
	return (a & 0xFFFF00FF) | (b & 0x0000FF00);
}

int main()
{
	
	int a, b;
	printf("Enter two numbers: ");
	scanf("%d %d", &a, &b);

	unsigned int z = merge_bytes(a, b);
	printf("%d and %d merged: %d\n", a, b, z);
	printf("%x and %x merged in hex: %x\n", a, b, z);

	a = 0xAABBCCDD;
	b = 0x11223344;
	z = merge_bytes(a, b);
	printf("%d and %d merged: %d\n", a, b, z);
	printf("%x and %x merged in hex: %x\n", a, b, z);
	
	
	return 0;
}
