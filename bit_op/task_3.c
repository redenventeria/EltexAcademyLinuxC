#include<stdio.h>
#include<stdlib.h>

// 1. Вывести двоичное представление целого положительного числа, используя битовые операции (число вводится с клавиатуры).

#define INT_SIZE 32

int is_non_zero(int a)
{
	return a != 0 ? 1 : 0;
}

unsigned int count_ones(int a)
{
	
	unsigned int ones = 0;
	
	unsigned int count, i;
        for(i = 1 << 31, count = 0; count < 32; i = (i >> 1), count++) {
		ones += is_non_zero(i & a); 
	} 

	return ones;
				                        
}

int main()
{
	int a;
	printf("Enter a number: ");
	scanf("%d", &a);

	unsigned int z = count_ones(a);
	printf("%d contains %d ones.\n", a, z);


	//Проверка для чисел от -32 до 32
	/*	
	for(int i = -32; i <= 32; i++) {		
		unsigned int z = count_ones(i);
		printf("%d contains %d ones.\n", i, z);
	}
	*/
	return 0;
	
}	
