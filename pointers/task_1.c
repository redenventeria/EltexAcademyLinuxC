#include<stdio.h>
#include<stdlib.h>

#define BUF_SIZE 30

int main()
{
	int a, b;
	char buf[BUF_SIZE];
	printf("Enter two numbers: ");
	fgets(buf, BUF_SIZE, stdin);
	while(sscanf(buf, "%d %d", &a, &b) != 2) {
		printf("Enter two numbers: ");
		fgets(buf, BUF_SIZE, stdin);
	}

	a = 0xAABBCCDD;
	b = 0x11223344;	
	char *ca = (char*)&a;
	char *cb = (char*)&b;

	ca[1] = cb[1];

	printf("%x\n", a);

}
