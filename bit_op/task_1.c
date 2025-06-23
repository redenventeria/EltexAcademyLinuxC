#include<stdio.h>
#include<stdlib.h>

// 1. Вывести двоичное представление целого положительного числа, используя битовые операции (число вводится с клавиатуры).

#define INT_SIZE 32

char* get_base_2(unsigned int a)
{

	char *s = malloc(sizeof(char) * (INT_SIZE + 1));
	
	unsigned int count, i;
        for(i = 1, count = 0; count < 32; i = (i << 1), count++) {
		//printf("bit %u: %d\n", count, ((i & a) >> count)); // Более детальный вывод с нумерацией
		printf("%u", ((i & a) >> count));
		s[count] = ((i & a) >> count) + '0';
        }

	s[count] = '\0';

	return s;
				                        
}

int main()
{
	unsigned int a;
	printf("Enter a positive number: ");
	scanf("%d", &a);
	
	char *s = get_base_2(a);
	printf("%s\n", s);
	free(s);
	printf("%d", s);
	return 0;
}
