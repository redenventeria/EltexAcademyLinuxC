#include<stdio.h>
#include<stdlib.h>

// 1. Вывести двоичное представление целого положительного числа, используя битовые операции (число вводится с клавиатуры).

#define INT_SIZE 32

int is_non_zero(int a)
{
	return a != 0 ? 1 : 0;
}

char* get_base_2(unsigned int a)
{

	char *s = malloc(sizeof(char) * (INT_SIZE + 1));
	
	unsigned int count, i;
        for(i = 1 << 31, count = 0; count < 32; i = (i >> 1), count++) {
		s[count] = is_non_zero(i & a) + '0'; 
        }

	s[count] = '\0';

	return s;
				                        
}

int main()
{
	unsigned int a;
	printf("Enter positive  number: ");
	scanf("%d", &a);

	char *s = get_base_2(a);
	printf("%d in binary: %s\n", a, s);
	free(s);


	//Проверка для чисел от 1 до 32
        /*	
	for(unsigned int i = 1; i <= 32; i++) {
		char *s = get_base_2(i);
		printf("%d in binary: %s\n", i, s);
		free(s);
	}
	return 0;
	*/
}
