#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const char menu[] = "1) Добаить абонента\n2) Удалить абонента\n3) Поиск всех абонентов по имени\n4) Вывод всех записей\n5) Выход\nВведите команду: ";

#define BUF_SIZE 10

typedef struct {
	char name[10];
	char second_name[10];
	char tel[10];
} abonent;

char get_command()
{
	char buf[BUF_SIZE];
	do {
		printf("%s", menu);
		fgets(buf, BUF_SIZE, stdin);

		if(buf[strlen(buf) - 1] != '\n')
			while(getchar() != '\n') {}
		
		if(strlen(buf) != 2)
			buf[0] = ' ';

		// printf("read %lu\n bytes", strlen(buf) + 1);

		if (!('1' <= buf[0] && buf[0] <= '5'))
			printf("Такой опции нет, попробуйте ещё раз.\n\n");
	} while (!('1' <= buf[0] && buf[0] <= '5'));

	return buf[0];
}

void add_abonent() {}
void del_abonent() {}
void search_abonent() {}
void print_all_abonents() {}


int main()
{	
	abonent m[100];
	
	char command = 0;

	while(command != '5')
	{
		command = get_command();

		switch(command) {
			case '1':
				add_abonent(); break;
			case '2':
				del_abonent(); break;
			case '3':
				search_abonent(); break;
			case '4':
				print_all_abonents(); break;
			case '5':
				break;
			default:
				printf("ERROR: unreachable branch!");
		}
		printf("\n");
	}

	if(command == '5')
		printf("OK, closing...\n");
	else
		printf("ERROR: branch is unreachable!");

}
