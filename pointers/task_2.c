#include<stdio.h>
#include<stdlib.h>

#define BUF_SIZE 30

int main(void)
{
	float x = 5.0;
	printf("x = %f, ", x);
	float y = 6.0;
	printf("y = %f\n", y);
	float *xp = &y; // : отредактируйте эту строку, и только правую часть уравнения
	float *yp = &y;
	printf("Результат: %f\n", *xp + *yp);

	return 0;
}
