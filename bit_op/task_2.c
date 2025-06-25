#include<stdio.h>
#include<stdlib.h>

// 1. Вывести двоичное представление целого положительного числа, используя битовые операции (число вводится с клавиатуры).

#define INT_SIZE 32

int is_non_zero(int a)
{
	return a != 0 ? 1 : 0;
}

char* get_base_2(int a)
{

	char *s = malloc(sizeof(char) * (INT_SIZE + 1));
	
	unsigned int count, i;
        for(i = 1 << 31, count = 0; count < 32; i = (i >> 1), count++) {
		//printf("bit %u: %d\n", count, ((i & a) >> count)); // Более детальный вывод с нумерацией
		//printf("%d\n", ((i & a) >> count));
		s[count] = is_non_zero(i & a) + '0'; // Сводим ненулевые биты к 1, чтобы не было проблем с отрицательными
        }

	s[count] = '\0';

	return s;
				                        
}

int main()
{
	int a;
	printf("Enter a negative number: ");
	scanf("%d", &a);
	if (a >= 0) {
		printf("Number is incorrect, aborting");
		return 0;

	}

	char *s = get_base_2(a);
	printf("%d in binary: %s\n", a, s);
	free(s);


	/*Проверка для чисел от -1 до -32
	
	for(int i = -1; i >= -32; i--) {
		char *s = get_base_2(i);
		printf("%d in binary: %s\n", i, s);
		free(s);
	}
	*/
	return 0;
}
